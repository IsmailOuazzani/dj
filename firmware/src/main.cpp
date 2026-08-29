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

constexpr uint8_t CC_EQ_LOW       = 22;
constexpr uint8_t NOTE_BROWSE_CW  = 0x40;
constexpr uint8_t NOTE_BROWSE_CCW = 0x41;

constexpr unsigned long POT_SEND_INTERVAL_MS = 5;

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

struct Encoder {
  uint8_t a_pin;        // MCP pin, quadrature phase A
  uint8_t b_pin;        // MCP pin, quadrature phase B
  uint8_t note_cw;
  uint8_t note_ccw;
  uint8_t channel;
  int last_a = HIGH;
  int last_b = HIGH;
};

Pot pots[] = {
  {0, CC_EQ_LOW, 1},   // deck A low EQ knob — CD4067 channel labeled 0 (Y0)
};

Encoder encoders[] = {
  // Deck A wheel — Bourns PEC11R-4215F-S0024, phase A on MCP pad A3, phase B on A4.
  // https://www.digikey.ca/en/products/detail/bourns-inc/PEC11R-4215F-S0024/4499665
  {3, 4, NOTE_BROWSE_CW, NOTE_BROWSE_CCW, 1},
};

void setup() {
  usb_midi.setStringDescriptor("ISMA 3000");
  Serial.begin(115200);
  analogReadResolution(12);

  for (uint8_t s : MUX_S_PINS) pinMode(s, OUTPUT);

  mcp_ok = mcp.begin_I2C(MCP_ADDR);
  for (auto& e : encoders) {
    mcp.pinMode(e.a_pin, INPUT_PULLUP);
    mcp.pinMode(e.b_pin, INPUT_PULLUP);
  }

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

  for (auto& e : encoders) {
    // Half-step decode: fire once per detent on A's falling edge; direction
    // from B's level at that moment.
    const int a = mcp.digitalRead(e.a_pin);
    const int b = mcp.digitalRead(e.b_pin);
    if (a != e.last_a || b != e.last_b) {
      if (a != e.last_a && a == LOW) {
        const uint8_t note = (b == HIGH) ? e.note_cw : e.note_ccw;
        MIDI.sendNoteOn(note, 127, e.channel);
        Serial.printf("ENC ch%u: %s\n", e.channel, (b == HIGH) ? "CW" : "CCW");
      }
      e.last_a = a;
      e.last_b = b;
    }
  }
}
