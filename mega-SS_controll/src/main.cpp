#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

Adafruit_PN532 nfc(2);

uint8_t success;
uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength;
const uint8_t ttl_sqr = 2;
uint8_t sqr_ss[ttl_sqr] = {2,3};
String sqr_data[ttl_sqr];

void setup(void)
{
  Serial.begin(9600);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
  {
    Serial.print("! board 1");
    while (1);
  }

  long int time = millis();

  success = nfc.readPassiveTargetID_board(sqr_data,ttl_sqr,sqr_ss,PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success)
  {
    uint8_t data[32];
    nfc.mifareultralight_ReadPage(4, data);
    Serial.println("");
  }

    //chess piece possition buffer access
    Serial.println("recieved chess piece possition buffer,");
    for(uint8_t i = 0;i<ttl_sqr;i++){
      Serial.print(sqr_data[i]);
      Serial.println();
    }

  Serial.print(millis() - time);
  Serial.println("ms");
}

void loop(void)
{
  delay(100);
}
