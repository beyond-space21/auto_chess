#include <Arduino.h>
#include <Wire.h>

// 0 -> RX
// 1 -> TX
// 2 -> SS
// 20 -> SDA
// 21 -> SCL

int pin_[64] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68};
byte data = 0;
byte pin = 0;

void receiveEvent(int size)
{
  while (Wire.available())
  {
    data = Wire.read();
    bool mode = (data)>>7;
    data = (data)^128;

    if(mode){// pin_set
      pin = data;
    }else{// on/off
      digitalWrite(pin,data);
    }

    Serial.print(mode);
    Serial.print(' ');
    Serial.println(data);
  }
}

void setup()
{
  Wire.begin(0x8);
  Serial.begin(9600);

  for (byte i = 0; i < 64; i++)
  {
    pinMode(pin_[i], OUTPUT);
    digitalWrite(pin_[i],0);
  }

  Wire.onReceive(receiveEvent);
  
  Serial.println(0);
}

void loop()
{
  delay(300);
}