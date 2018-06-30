#include <Arduino.h>
#include <SPI.h>

uint8_t ledPin = 13;

void setup(void)
{
  Serial.begin(115200);
pinMode(ledPin, OUTPUT);
}

void loop(void)
{
Serial.println("boop");
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}
