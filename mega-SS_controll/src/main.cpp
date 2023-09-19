#include <nfc.h>

#define own

#ifndef own
  #include <PN532_SPI.h>
  #include <PN532.h>
  PN532_SPI pn532spi(SPI, 10);
  PN532 nfc(pn532spi);
#endif

uint8_t sqr_pins[2] = {2,3};

void setup(){
  Serial.begin(9600);
#ifndef own
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion(sqr_pins);
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
#else

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000,LSBFIRST,SPI_MODE0));
  uint32_t versiondata = getFirmwareVersion(sqr_pins);
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }
Serial.println(">> board OK..!");
#endif
}

void loop(){
  delay(10);
}