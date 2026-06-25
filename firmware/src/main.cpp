#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

constexpr uint8_t POT_PIN = A0;
constexpr uint8_t CC_NUMBER = 21;
constexpr uint8_t CC_CHANNEL = 1;
constexpr unsigned long SEND_INTERVAL_MS = 5;

constexpr uint8_t BUTTON_PIN = D2;
constexpr uint8_t BUTTON_NOTE = 0x24;
constexpr unsigned long DEBOUNCE_MS = 20;

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup() {
  usb_midi.setStringDescriptor("ISMA 3000");

  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  if (!TinyUSBDevice.mounted()) return;

  // ---- Pot ----
  static int last_sent = -1;
  static unsigned long last_send_ms = 0;

  const unsigned long now = millis();
  if (now - last_send_ms >= SEND_INTERVAL_MS) {
    const int value7 = analogRead(POT_PIN) >> 5;
    if (value7 != last_sent) {
      MIDI.sendControlChange(CC_NUMBER, value7, CC_CHANNEL);
      last_sent = value7;
      last_send_ms = now;

      Serial.print("CC21: ");
      Serial.println(value7);
    }
  }

  // ---- Button (falling-edge = press, INPUT_PULLUP inverts logic) ----
  static int last_stable = HIGH;
  static int last_read = HIGH;
  static unsigned long last_change_ms = 0;

  const int reading = digitalRead(BUTTON_PIN);
  if (reading != last_read) {
    last_read = reading;
    last_change_ms = now;
  }
  if (now - last_change_ms >= DEBOUNCE_MS && reading != last_stable) {
    last_stable = reading;
    if (reading == LOW) {
      MIDI.sendNoteOn(BUTTON_NOTE, 127, CC_CHANNEL);
      Serial.println("BTN: press");
    }
  }
}
