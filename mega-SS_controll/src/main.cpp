#include <Arduino.h>
#include <cmd.h>
#include <SPI.h>
// #include <Adafruit_SPIDevice.h>

#define ss 2

// Adafruit_SPIDevice *spi_dev = new Adafruit_SPIDevice(ss, 13, 12, 11, 1000000, SPI_BITORDER_LSBFIRST, SPI_MODE0);
// Adafruit_SPIDevice sp(ss, 1000000, SPI_BITORDER_LSBFIRST, SPI_MODE0);


byte packet_buffer [64];

uint16_t timeout = 20;

byte pn532ack[] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};

//SPI functions
  bool write(const uint8_t *buffer, int len){
    Serial.print("func write()");
    digitalWrite(ss,LOW);
    // byte buff[len];
    for (int i = 0; i < len; i++) {
      // buff[i] = buffer[i];
      SPI.transfer(buffer[i]);
    }
    // SPI.transfer(buff,len);
    digitalWrite(ss,HIGH);
    Serial.println("func write() end");
    return true;
  }

  bool write_then_read(const uint8_t *write_buffer, size_t write_len, uint8_t *read_buffer, size_t read_len, uint8_t sendvalue=(0xFF)){
    digitalWrite(ss,LOW);
    //write
    for (size_t i = 0; i < write_len; i++) {
      SPI.transfer(write_buffer[i]);
    }
    //read
    for (size_t i = 0; i < read_len; i++) {
    read_buffer[i] = SPI.transfer(sendvalue);
  }
    digitalWrite(ss,HIGH);

    return true;
  }  
//SPI functions

void writecommand(uint8_t *cmd, int cmdlen)
{
  Serial.println("func writecommand()");
  uint8_t checksum;
  uint8_t packet[9 + cmdlen];
  uint8_t *p = packet;
  cmdlen++;

  p[0] = PN532_SPI_DATAWRITE;
  p++;
  p[0] = PN532_PREAMBLE;
  p++;
  p[0] = PN532_STARTCODE1;
  p++;
  p[0] = PN532_STARTCODE2;
  p++;
  checksum = PN532_PREAMBLE + PN532_STARTCODE1 + PN532_STARTCODE2;
  p[0] = cmdlen;
  p++;
  p[0] = ~cmdlen + 1;
  p++;
  p[0] = PN532_HOSTTOPN532;
  p++;
  checksum += PN532_HOSTTOPN532;

  for (uint8_t i = 0; i < cmdlen - 1; i++)
  {
    p[0] = cmd[i];
    p++;
    checksum += cmd[i];
  }

  p[0] = ~checksum;
  p++;
  p[0] = PN532_POSTAMBLE;
  p++;

  write(packet, 8+cmdlen);
}

bool readack(){
  uint8_t ackbuff[6];
  uint8_t cmd = PN532_SPI_DATAREAD;
  write_then_read(&cmd, 1, ackbuff, 6);
  Serial.println(ackbuff[0]);
  return (0 == memcmp((char *)ackbuff, (char *)pn532ack, 6));
}

bool isready()
{
  uint8_t cmd = PN532_SPI_STATREAD;
  uint8_t reply;
  write_then_read(&cmd, 1, &reply, 1);
  return reply == PN532_SPI_READY;
}

bool waitready(uint16_t timeout)
{
  while (!isready())
  {
    if (timeout != 0)
    {
      timeout--;
    }else
     return false;
    Serial.println("getting ACK!");
    delay(1);
  }
  Serial.println("foud ACK!");
  return true;
}

bool checkACK(uint8_t *cmd, uint8_t cmdlen, uint16_t timeout=1000){
  Serial.println("func checkACK()");
  writecommand(cmd, cmdlen);
  if (!waitready(timeout)) {
    return false;
  }
  if (!readack()) {
    return false;
  }

  return true;
}


void setup(void)
{
  Serial.begin(9600);
  Serial.println("System initialized");
  pinMode(ss,OUTPUT);
  digitalWrite(ss,HIGH);
  SPI.begin();
  
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  delay(3000);
  packet_buffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;
  Serial.println(checkACK(packet_buffer,1));
  SPI.endTransaction();
  Serial.println("copleted.");
}

void loop()
{
  delay(300);
}