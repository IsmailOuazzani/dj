#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_MCP23X17.h>
#include <MIDI.h>

// Analog signals reach ADC A0 via the CD74HC4067 mux.
// S0-S3 pick the channel; SIG is the analog output.
constexpr uint8_t MUX_SIG_PIN = A0;
constexpr uint8_t MUX_S_PINS[4] = {D2, D3, D4, D5};

// Digital signals reach the MCP23017s over I²C (STEMMA QT to A4/A5).
// One expander per deck. Both boards are Adafruit 5346, which ship strapped to
// 0x20; the deck-A board has its `D0` solder jumper bridged to VIN, moving it to
// 0x21. (`D0`/`D1`/`D2` are the address pads near the chip — NOT the `A0`-`A7`
// GPIO pads on the edge. See ../hardware-docs/adafruit-5346-mcp23017-stemma.md.)
constexpr uint8_t CHIP_B = 0;  // right deck — un-bridged board
constexpr uint8_t CHIP_A = 1;  // left deck  — `D0` bridged
constexpr uint8_t MCP_ADDR[2] = {0x20, 0x21};

// Deck B jog wheel — rotary encoder A/B on native GPIO with pin-change interrupts.
// Common pin wired to GND; INPUT_PULLUP enabled below.
constexpr uint8_t JOG_PIN_A = D8;  // GPIO20
constexpr uint8_t JOG_PIN_B = D9;  // GPIO21

// Deck A on MIDI channel 1, deck B on channel 2 — mirrored numbering.
constexpr uint8_t CC_FILTER    = 7;
constexpr uint8_t CC_VOLUME    = 20;
constexpr uint8_t CC_TEMPO     = 21;
constexpr uint8_t CC_JOG       = 22;
constexpr uint8_t CC_BASS      = 23;
constexpr uint8_t CC_GAIN      = 24;
// Crossfader and the FX knob are global, not per-deck: they ride channel 1 with
// their own CC numbers rather than being mirrored across both channels.
constexpr uint8_t CC_CROSSFADER = 25;
constexpr uint8_t CC_FX_SUPER   = 26;
constexpr uint8_t NOTE_PLAY    = 0x24;
constexpr uint8_t NOTE_SYNC    = 0x25;
constexpr uint8_t NOTE_LOOP_IN  = 0x26;
constexpr uint8_t NOTE_LOOP_OUT = 0x27;
constexpr uint8_t NOTE_LOOP_EXIT = 0x28;
constexpr uint8_t NOTE_SHIFT   = 0x29;
constexpr uint8_t NOTE_PAD1    = 0x30;
constexpr uint8_t NOTE_PAD2    = 0x31;
constexpr uint8_t NOTE_PAD3    = 0x32;
constexpr uint8_t NOTE_PAD4    = 0x33;

constexpr unsigned long POT_SEND_INTERVAL_MS = 5;
constexpr unsigned long DEBOUNCE_MS = 20;

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);
Adafruit_MCP23X17 mcp[2];
bool mcp_ok[2] = {false, false};
bool boot_diag_printed = false;

int mux_read(uint8_t ch) {
  for (int i = 0; i < 4; i++) digitalWrite(MUX_S_PINS[i], (ch >> i) & 1);
  delayMicroseconds(50);  // CD4067 tON max ~15ns but RC on SIG line needs longer to settle
  return analogRead(MUX_SIG_PIN);
}

// Quadrature decode table: QUAD_TABLE[prev_AB][curr_AB] = +1 (CW), -1 (CCW), 0 (invalid/still).
// AB bit encoding: bit1=A, bit0=B. CW sequence: 00→10→11→01→00.
static const int8_t QUAD_TABLE[4][4] = {
  { 0, -1,  1,  0},  // prev AB=00
  { 1,  0,  0, -1},  // prev AB=01
  {-1,  0,  0,  1},  // prev AB=10
  { 0,  1, -1,  0},  // prev AB=11
};

volatile uint8_t jog_last_ab = 0;
volatile int32_t jog_delta = 0;

// Fires on any edge of either phase — decodes on the fly so no transition is missed
// even during fast spins. Kept minimal (no Serial, no MIDI) so the ISR stays quick.
void jog_isr() {
  uint8_t ab = (digitalRead(JOG_PIN_A) ? 2 : 0) |
               (digitalRead(JOG_PIN_B) ? 1 : 0);
  jog_delta += QUAD_TABLE[jog_last_ab][ab];
  jog_last_ab = ab;
}

struct Pot {
  uint8_t mux_ch;
  uint8_t cc;
  uint8_t channel;
  int last_sent = -1;
  unsigned long last_send_ms = 0;
};

struct Button {
  uint8_t chip;  // CHIP_A / CHIP_B — index into mcp[]
  uint8_t mcp_pin;
  uint8_t note;
  uint8_t channel;
  int last_stable = HIGH;
  int last_read = HIGH;
  unsigned long last_change_ms = 0;
};

// All on the SparkFun CD74HC4067 breakout ("red board"); numbers are mux channels.
// The deck B tempo slider that used to sit on channel 2 was removed in the rewiring
// that added this bank — channel 2 is now the FX knob.
Pot pots[] = {
  // Sliding pots
  {15, CC_CROSSFADER, 1},  // crossfader        — global, ch 1
  {14, CC_VOLUME,     1},  // deck A volume fader
  {13, CC_VOLUME,     2},  // deck B volume fader
  // Rotary pots
  {10, CC_FILTER,     1},  // deck A filter
  { 9, CC_FILTER,     2},  // deck B filter
  { 7, CC_BASS,       1},  // deck A bass
  { 5, CC_BASS,       2},  // deck B bass
  { 4, CC_GAIN,       1},  // deck A gain/trim
  { 3, CC_GAIN,       2},  // deck B gain/trim
  { 2, CC_FX_SUPER,   1},  // effect filter knob — global, ch 1
};

Button buttons[] = {
  // Deck B (right) — expander at 0x20.
  {CHIP_B, 10, NOTE_PLAY,      2}, // deck B play/pause — MCP pad labeled B2 (GPB2)
  {CHIP_B, 11, NOTE_SHIFT,     2}, // deck B shift      — MCP pad labeled B3 (GPB3)
  {CHIP_B, 12, NOTE_SYNC,      2}, // deck B sync       — MCP pad labeled B4 (GPB4)
  {CHIP_B, 13, NOTE_LOOP_EXIT, 2}, // deck B loop exit  — MCP pad labeled B5 (GPB5)
  {CHIP_B, 14, NOTE_LOOP_IN,   2}, // deck B loop in    — MCP pad labeled B6 (GPB6)
  {CHIP_B, 15, NOTE_LOOP_OUT,  2}, // deck B loop out   — MCP pad labeled B7 (GPB7)
  {CHIP_B,  4, NOTE_PAD4,      2}, // deck B pad 4      — MCP pad labeled A4 (GPA4)
  {CHIP_B,  5, NOTE_PAD3,      2}, // deck B pad 3      — MCP pad labeled A5 (GPA5)
  {CHIP_B,  6, NOTE_PAD2,      2}, // deck B pad 2      — MCP pad labeled A6 (GPA6)
  {CHIP_B,  7, NOTE_PAD1,      2}, // deck B pad 1      — MCP pad labeled A7 (GPA7)

  // Deck A (left) — expander at 0x21. Same notes as deck B, mirrored onto ch 1.
  {CHIP_A, 15, NOTE_LOOP_IN,   1}, // deck A loop in    — MCP pad labeled B7 (GPB7)
  {CHIP_A, 14, NOTE_LOOP_OUT,  1}, // deck A loop out   — MCP pad labeled B6 (GPB6)
  {CHIP_A, 13, NOTE_LOOP_EXIT, 1}, // deck A loop toggle— MCP pad labeled B5 (GPB5)
  {CHIP_A, 12, NOTE_SYNC,      1}, // deck A sync       — MCP pad labeled B4 (GPB4)
  {CHIP_A, 11, NOTE_SHIFT,     1}, // deck A shift      — MCP pad labeled B3 (GPB3)
  {CHIP_A,  3, NOTE_PLAY,      1}, // deck A start      — MCP pad labeled A3 (GPA3)
  {CHIP_A,  4, NOTE_PAD1,      1}, // deck A pad 1      — MCP pad labeled A4 (GPA4)
  {CHIP_A,  5, NOTE_PAD2,      1}, // deck A pad 2      — MCP pad labeled A5 (GPA5)
  {CHIP_A,  6, NOTE_PAD3,      1}, // deck A pad 3      — MCP pad labeled A6 (GPA6)
  {CHIP_A,  7, NOTE_PAD4,      1}, // deck A pad 4      — MCP pad labeled A7 (GPA7)
};

void setup() {
  usb_midi.setStringDescriptor("ISMA 3000");
  Serial.begin(115200);
  analogReadResolution(12);

  for (uint8_t s : MUX_S_PINS) pinMode(s, OUTPUT);

  for (uint8_t i = 0; i < 2; i++) mcp_ok[i] = mcp[i].begin_I2C(MCP_ADDR[i]);
  // Skip pins on an expander that never answered: configuring (and later reading)
  // an absent chip yields garbage that reads as phantom presses.
  for (auto& b : buttons) {
    if (mcp_ok[b.chip]) mcp[b.chip].pinMode(b.mcp_pin, INPUT_PULLUP);
  }

  pinMode(JOG_PIN_A, INPUT_PULLUP);
  pinMode(JOG_PIN_B, INPUT_PULLUP);
  jog_last_ab = (digitalRead(JOG_PIN_A) ? 2 : 0) |
                (digitalRead(JOG_PIN_B) ? 1 : 0);
  attachInterrupt(digitalPinToInterrupt(JOG_PIN_A), jog_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(JOG_PIN_B), jog_isr, CHANGE);

  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  if (!TinyUSBDevice.mounted()) return;
  if (!boot_diag_printed) {
    Serial.printf("boot: MCP A(0x%02X) %s | MCP B(0x%02X) %s\n",
                  MCP_ADDR[CHIP_A], mcp_ok[CHIP_A] ? "OK" : "OFFLINE",
                  MCP_ADDR[CHIP_B], mcp_ok[CHIP_B] ? "OK" : "OFFLINE");
    boot_diag_printed = true;
  }
  const unsigned long now = millis();

  static unsigned long last_diag_ms = 0;
  if (now - last_diag_ms >= 3000) {
    Serial.printf("MCP A %s | B %s\n", mcp_ok[CHIP_A] ? "OK" : "OFFLINE",
                  mcp_ok[CHIP_B] ? "OK" : "OFFLINE");
    last_diag_ms = now;
  }

  for (auto& p : pots) {
    if (now - p.last_send_ms < POT_SEND_INTERVAL_MS) continue;
    const int value7 = mux_read(p.mux_ch) >> 5;
    if (abs(value7 - p.last_sent) < 2) continue;
    MIDI.sendControlChange(p.cc, value7, p.channel);
    p.last_sent = value7;
    p.last_send_ms = now;
    Serial.printf("CC%u ch%u: %d\n", p.cc, p.channel, value7);
  }

  noInterrupts();
  int32_t delta = jog_delta;
  jog_delta = 0;
  interrupts();
  if (delta != 0) {
    // Two's-complement relative for Mixxx SelectKnob: 1 = +1 CW, 127 = -1 CCW.
    // One MIDI per quadrature transition — Mixxx handles the volume fine over USB.
    const int32_t n = delta > 0 ? delta : -delta;
    const uint8_t value = delta > 0 ? 1 : 127;
    for (int32_t i = 0; i < n; i++) MIDI.sendControlChange(CC_JOG, value, 2);
    Serial.printf("JOG delta: %ld\n", (long)delta);
  }

  for (auto& b : buttons) {
    if (!mcp_ok[b.chip]) continue;
    const int reading = mcp[b.chip].digitalRead(b.mcp_pin);
    if (reading != b.last_read) {
      b.last_read = reading;
      b.last_change_ms = now;
    }
    if (now - b.last_change_ms < DEBOUNCE_MS) continue;
    if (reading == b.last_stable) continue;
    b.last_stable = reading;
    // Send both press (NoteOn) AND release (NoteOff). Mixxx's <Normal/> option
    // treats value 0 / NoteOff as "released"; without the release edge, controls
    // like loop_in / loop_out / hotcue_*_activate stay "held" forever and their
    // press-and-hold branches (move loop point, preview cue) fire instead of the
    // one-shot branches (set loop point, jump to cue).
    if (reading == LOW) {
      MIDI.sendNoteOn(b.note, 127, b.channel);
      Serial.printf("BTN ch%u note 0x%02X: press\n", b.channel, b.note);
    } else {
      MIDI.sendNoteOff(b.note, 0, b.channel);
      Serial.printf("BTN ch%u note 0x%02X: release\n", b.channel, b.note);
    }
  }
}
