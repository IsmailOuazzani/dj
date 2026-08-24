#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_MCP23X17.h>
#include <MIDI.h>

// Analog signals reach ADC A0 via the CD74HC4067 mux.
// S0-S3 pick the channel; SIG is the analog output.
constexpr uint8_t MUX_SIG_PIN = A0;
constexpr uint8_t MUX_S_PINS[4] = {D2, D3, D4, D5};

// Digital signals reach the MCP23017 over I²C (STEMMA QT to A4/A5).
// Board is strapped to the default I²C address.
constexpr uint8_t MCP_ADDR = 0x20;

// Deck A on MIDI channel 1, deck B on channel 2 — mirrored numbering.
constexpr uint8_t CC_FILTER = 7;
constexpr uint8_t CC_VOLUME = 20;
constexpr uint8_t CC_TEMPO  = 21;
constexpr uint8_t NOTE_PLAY = 0x24;
constexpr uint8_t NOTE_SYNC = 0x25;
constexpr uint8_t NOTE_PAD1 = 0x30;

constexpr unsigned long POT_SEND_INTERVAL_MS = 5;
constexpr unsigned long DEBOUNCE_MS = 20;

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);
Adafruit_MCP23X17 mcp;
bool mcp_ok = false;
bool boot_diag_printed = false;

int mux_read(uint8_t ch) {
  for (int i = 0; i < 4; i++) digitalWrite(MUX_S_PINS[i], (ch >> i) & 1);
  delayMicroseconds(5);  // let mux settle before the ADC samples
  return analogRead(MUX_SIG_PIN);
}

struct Pot {
  uint8_t mux_ch;
  uint8_t cc;
  uint8_t channel;
  int last_sent = -1;
  unsigned long last_send_ms = 0;
};

struct Button {
  uint8_t mcp_pin;
  uint8_t note;
  uint8_t channel;
  int last_stable = HIGH;
  int last_read = HIGH;
  unsigned long last_change_ms = 0;
};

Pot pots[] = {
  {15, CC_TEMPO, 1},   // deck A tempo slider — CD4067 channel labeled 15 (Y15)
};

Button buttons[] = {
  {0, NOTE_SYNC, 1},   // deck A sync       — MCP pad labeled A0 (GPA0)
  {1, NOTE_PLAY, 1},   // deck A play/pause — MCP pad labeled A1 (GPA1)
  {2, NOTE_PAD1, 1},   // deck A pad 1      — MCP pad labeled A2 (GPA2)
};

void setup() {
  usb_midi.setStringDescriptor("ISMA 3000");
  Serial.begin(115200);
  analogReadResolution(12);

  for (uint8_t s : MUX_S_PINS) pinMode(s, OUTPUT);

  mcp_ok = mcp.begin_I2C(MCP_ADDR);
  for (auto& b : buttons) mcp.pinMode(b.mcp_pin, INPUT_PULLUP);

  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  if (!TinyUSBDevice.mounted()) return;
  if (!boot_diag_printed) {
    Serial.printf("MCP %s (addr 0x%02X)\n", mcp_ok ? "OK" : "OFFLINE", MCP_ADDR);
    boot_diag_printed = true;
  }
  const unsigned long now = millis();

  for (auto& p : pots) {
    if (now - p.last_send_ms < POT_SEND_INTERVAL_MS) continue;
    const int value7 = mux_read(p.mux_ch) >> 5;
    if (value7 == p.last_sent) continue;
    MIDI.sendControlChange(p.cc, value7, p.channel);
    p.last_sent = value7;
    p.last_send_ms = now;
    Serial.printf("CC%u ch%u: %d\n", p.cc, p.channel, value7);
  }

  for (auto& b : buttons) {
    const int reading = mcp.digitalRead(b.mcp_pin);
    if (reading != b.last_read) {
      b.last_read = reading;
      b.last_change_ms = now;
    }
    if (now - b.last_change_ms < DEBOUNCE_MS) continue;
    if (reading == b.last_stable) continue;
    b.last_stable = reading;
    if (reading == LOW) {
      MIDI.sendNoteOn(b.note, 127, b.channel);
      Serial.printf("BTN ch%u: press\n", b.channel);
    }
  }
}
