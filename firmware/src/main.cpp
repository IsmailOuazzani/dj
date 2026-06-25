#include <Arduino.h>

constexpr uint8_t POT_PIN = A0;
constexpr unsigned long PRINT_INTERVAL_MS = 100;

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  int value = analogRead(POT_PIN);
  Serial.print("A0: ");
  Serial.println(value);
  delay(PRINT_INTERVAL_MS);
}
