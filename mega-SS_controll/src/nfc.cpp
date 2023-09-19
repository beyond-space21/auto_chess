#include <nfc.h>

#define _ss 10

uint8_t command;
uint8_t pn532_packetbuffer[64];

uint32_t getFirmwareVersion(uint8_t *sqr_pins)
{
    Serial.println("getfiremware()");
    uint32_t response;

    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

    if (writeCommand(sqr_pins,pn532_packetbuffer, 1)) {
        return 0;
    }

    // read data packet
    int16_t status = readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer));
    if (0 > status) {
        return 0;
    }

    response = pn532_packetbuffer[0];
    response <<= 8;
    response |= pn532_packetbuffer[1];
    response <<= 8;
    response |= pn532_packetbuffer[2];
    response <<= 8;
    response |= pn532_packetbuffer[3];

    return response;
}

void writeFrame(uint8_t *sqr_pins, const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    Serial.println("writeframe()");
    digitalWrite(_ss, LOW);
    delay(2); // wake up PN532

    write(DATA_WRITE);
    write(PN532_PREAMBLE);
    write(PN532_STARTCODE1);
    write(PN532_STARTCODE2);

    uint8_t length = hlen + blen + 1; // length of data field: TFI + DATA
    write(length);
    write(~length + 1); // checksum of length

    write(PN532_HOSTTOPN532);
    uint8_t sum = PN532_HOSTTOPN532; // sum of TFI + DATA

    for (uint8_t i = 0; i < hlen; i++)
    {
        write(header[i]);
        sum += header[i];
    }
    for (uint8_t i = 0; i < blen; i++)
    {
        write(body[i]);
        sum += body[i];
    }

    uint8_t checksum = ~sum + 1; // checksum of TFI + DATA
    write(checksum);
    write(PN532_POSTAMBLE);

    digitalWrite(_ss, HIGH);
}

int16_t readResponse(uint8_t buf[], uint8_t len, uint16_t timeout){
    Serial.println("readresponse()");
    uint16_t time = 0;
    while (!isReady())
    {
        delay(1);
        time++;
        if (time > timeout)
        {
            return PN532_TIMEOUT;
        }
    }

    digitalWrite(_ss, LOW);
    delay(1);

    int16_t result;
    do
    {
        write(DATA_READ);

        if (0x00 != read() || // PREAMBLE
            0x00 != read() || // STARTCODE1
            0xFF != read()    // STARTCODE2
        )
        {

            result = PN532_INVALID_FRAME;
            break;
        }

        uint8_t length = read();
        if (0 != (uint8_t)(length + read()))
        { // checksum of length
            result = PN532_INVALID_FRAME;
            break;
        }

        uint8_t cmd = command + 1; // response command
        if (PN532_PN532TOHOST != read() || (cmd) != read())
        {
            result = PN532_INVALID_FRAME;
            break;
        }


        length -= 2;
        if (length > len)
        {
            for (uint8_t i = 0; i < length; i++)
            {
            }
            read();
            read();
            result = PN532_NO_SPACE; // not enough space
            break;
        }

        uint8_t sum = PN532_PN532TOHOST + cmd;
        for (uint8_t i = 0; i < length; i++)
        {
            buf[i] = read();
            sum += buf[i];

        }

        uint8_t checksum = read();
        if (0 != (uint8_t)(sum + checksum))
        {
            result = PN532_INVALID_FRAME;
            break;
        }
        read(); // POSTAMBLE

        result = length;
    } while (0);

    digitalWrite(_ss, HIGH);

    return result;
}

int8_t writeCommand(uint8_t *sqr_pins, const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    Serial.println("writecommand()");
    command = header[0];
    writeFrame(sqr_pins,header, hlen, body, blen);

    uint8_t timeout = PN532_ACK_WAIT_TIME;
    while (!isReady())
    {
        delay(1);
        timeout--;
        if (0 == timeout)
        {
            return -2;
        }
    }
    if (readAckFrame())
    {
        return PN532_INVALID_ACK;
    }
    return 0;
}

bool isReady()
{
    Serial.println("isready()");
    digitalWrite(_ss, LOW);
    write(STATUS_READ);
    uint8_t status = read() & 1;
    digitalWrite(_ss, HIGH);
    return status;
}

int8_t readAckFrame()
{
    Serial.println("readACKFrame()");
    const uint8_t PN532_ACK[] = {0, 0, 0xFF, 0, 0xFF, 0};
    uint8_t ackBuf[sizeof(PN532_ACK)];

    digitalWrite(_ss, LOW);
    delay(1);
    write(DATA_READ);

    for (uint8_t i = 0; i < sizeof(PN532_ACK); i++)
    {
        ackBuf[i] = read();
    }

    digitalWrite(_ss, HIGH);

    return memcmp(ackBuf, PN532_ACK, sizeof(PN532_ACK));
}

void write(uint8_t data)
{
    SPI.transfer(data);
}

uint8_t read()
{
    return SPI.transfer(0);
}