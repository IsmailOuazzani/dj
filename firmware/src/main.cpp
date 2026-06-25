#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

constexpr uint8_t POT_PIN = A0;
constexpr uint8_t CC_NUMBER = 21;
constexpr uint8_t CC_CHANNEL = 1;
constexpr unsigned long SEND_INTERVAL_MS = 5;

Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup() {
  // Names the USB-MIDI *interface* — what CoreMIDI on macOS (and therefore Mixxx)
  // uses to identify the device. Without this it falls back to a TinyUSB default
  // and Mixxx can't match the controller mapping XML.
  usb_midi.setStringDescriptor("ISMA 3000");

  Serial.begin(115200);
  analogReadResolution(12);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  static int last_sent = -1;
  static unsigned long last_send_ms = 0;

  if (!TinyUSBDevice.mounted()) return;

  const unsigned long now = millis();
  if (now - last_send_ms < SEND_INTERVAL_MS) return;

  const int value7 = analogRead(POT_PIN) >> 5;  // 12-bit → 7-bit
  if (value7 == last_sent) return;

  MIDI.sendControlChange(CC_NUMBER, value7, CC_CHANNEL);
  last_sent = value7;
  last_send_ms = now;

  Serial.print("CC21: ");
  Serial.println(value7);
}
