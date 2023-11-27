#include <Wire.h>

const int ledPin = 13; 

void setup() {
  Wire.begin(0x8);
  Serial.begin(9600);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int howMany) {
  // Serial.println(howMany);
  while (Wire.available()) {
    int c = Wire.read();
    Serial.println(c);
  }
}
void loop() {
  delay(100);
}