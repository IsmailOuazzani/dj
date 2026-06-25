#include <Arduino.h>

constexpr unsigned long BLINK_INTERVAL_MS = 500;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(BLINK_INTERVAL_MS);
  digitalWrite(LED_BUILTIN, LOW);
  delay(BLINK_INTERVAL_MS);
}
