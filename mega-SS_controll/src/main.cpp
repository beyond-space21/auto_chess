// https://github.com/Seeed-Studio/PN532

#include <nfc.h>
#include "EEPROM.h"

#define pin_conf

uint8_t sqr_pins[8] = {6, 62, A7, A15, 28, 44, 39, 23};
// uint8_t sqr_data[64];

void setup()
{

  pinMode(A15, OUTPUT);
  pinMode(A14, OUTPUT);

  pinMode(49,OUTPUT);
  digitalWrite(49,LOW);

  ss_set(A15);
  Serial.begin(9600);

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, LSBFIRST, SPI_MODE0));

  uint32_t versiondata = getFirmwareVersion();
  if (!versiondata)
  {
    Serial.print("err_200");
    while (1)
      ;
  }
  Serial.println(">> board OK1..!");

  // digitalWrite(49,HIGH);
  // delay(100);
  // digitalWrite(49,LOW);
  // digitalWrite(A15,HIGH);

  delay(100);
  // digitalWrite(6,HIGH);
  // delay(50);
  // digitalWrite(6,LOW);

  // ss_set(A14);
  // versiondata = getFirmwareVersion();
  // if (!versiondata)
  // {
  //   Serial.print("err_200");
  //   while (1)
  //     ;
  // }
  // Serial.println(">> board OK2..!");

  wakeup();
  SAMConfig();
  // delay(10);
  // while (1)
  //   ;
}

void loop()
{
  uint8_t success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
  uint8_t uidLength;

  success = readPassiveTargetID(uid, &uidLength);

  if (success)
  {

    if (uidLength == 7)
    {
      uint8_t data[32];
      success = mifareultralight_ReadPage(4, data);
      if (success)
      {
        Serial.println("read");
        // for(int i=0;i<32;i++)
        // Serial.print(data[i]);
        // delay(1000);
      }
      else
      {
        Serial.println("err_201");
      }
    }
    // Serial.println("l"); 
  }

  // while(1);
  delay(10);
}