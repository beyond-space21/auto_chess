// for SPI Communication
#include <SPI.h>


 
void setup(void) 
{
 Serial.begin(9600);
 Serial.println("System initialized");

 SPI.begin();
 SPI.beginTransaction(SPISettings(SPISettings(2000000, LSBFIRST, SPI_MODE0)));
 byte data = {}
}

void loop() 
{
  delay(300);
}
 
