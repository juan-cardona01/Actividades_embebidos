#include <Arduino.h>

#define LED 2

void setup() {
  Serial.begin(115200);
  pinMode (LED, OUTPUT);
}

void loop() {
  delay(1000);
  digitalWrite(LED, HIGH);
  Serial.println("Led encendido");

  delay(1000);
  digitalWrite(LED, LOW);
  Serial.println("Led apagado");
}