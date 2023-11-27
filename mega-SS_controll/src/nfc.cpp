#include <nfc.h>
#include "Arduino.h"

uint8_t _ss;
// uint8_t *sqr_pin;
// uint8_t *sqr_data;

uint8_t command;
uint8_t pn532_packetbuffer[64];

void ss_set(uint8_t _pin){
    _ss = _pin;
    // pn532_packetbuffer[64]=0;
    command = 0;
    for(int i=0;i<64;i++){
        pn532_packetbuffer[i]=0;
    }
}

uint32_t getFirmwareVersion()
{
    Serial.println("getfiremware()");
    uint32_t response;

    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

    if (writeCommand(pn532_packetbuffer, 1)) {
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

bool readPassiveTargetID(uint8_t *uid, uint8_t *uidLength){

    pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
    pn532_packetbuffer[2] = PN532_MIFARE_ISO14443A;

    if (writeCommand(pn532_packetbuffer, 3)) {
        return 0x0;  // command failed
    }

    // read data packet
    if (readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer)) < 0) {
        return 0x0;
    }

    // check some basic stuff
    /* ISO14443A card response should be in the following format:

      byte            Description
      -------------   ------------------------------------------
      b0              Tags Found
      b1              Tag Number (only one used in this example)
      b2..3           SENS_RES
      b4              SEL_RES
      b5              NFCID Length
      b6..NFCIDLen    NFCID
    */

    if (pn532_packetbuffer[0] != 1)
        return 0;

    uint16_t sens_res = pn532_packetbuffer[2];
    sens_res <<= 8;
    sens_res |= pn532_packetbuffer[3];

    // DMSG("ATQA: 0x");  DMSG_HEX(sens_res);
    // DMSG("SAK: 0x");  DMSG_HEX(pn532_packetbuffer[4]);
    // DMSG("\n");

    /* Card appears to be Mifare Classic */
    *uidLength = pn532_packetbuffer[5];

    for (uint8_t i = 0; i < pn532_packetbuffer[5]; i++) {
        uid[i] = pn532_packetbuffer[6 + i];
    }

    return 1;

}


bool mifareultralight_ReadPage (uint8_t page, uint8_t *buffer)
{
    /* Prepare the command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1;                   /* Card number */
    pn532_packetbuffer[2] = 0x30;     /* Mifare Read command = 0x30 */
    pn532_packetbuffer[3] = page;                /* Page Number (0..63 in most cases) */

    /* Send the command */
    if (writeCommand(pn532_packetbuffer, 4)) {
        return 0;
    }

    /* Read the response packet */
    readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    /* If byte 8 isn't 0x00 we probably have an error */
    if (pn532_packetbuffer[0] == 0x00) {
        /* Copy the 4 data bytes to the output buffer         */
        /* Block content starts at byte 9 of a valid response */
        /* Note that the command actually reads 16 bytes or 4  */
        /* pages at a time ... we simply discard the last 12  */
        /* bytes                                              */
        memcpy (buffer, pn532_packetbuffer + 1, 4);
    } else {
        return 0;
    }

    // Return OK signal
    return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void wakeup()
{
    digitalWrite(_ss, LOW);
    delay(2);
    digitalWrite(_ss, HIGH);
}

bool SAMConfig()
{
    pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
    pn532_packetbuffer[1] = 0x01; // normal mode;
    pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
    pn532_packetbuffer[3] = 0x01; // use IRQ pin!


    if (writeCommand(pn532_packetbuffer, 4))
        return false;

    return (0 < readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer)));
}

void writeFrame(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    // Serial.println("writeframe()");
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
    // Serial.println("readresponse()");
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

int8_t writeCommand(const uint8_t *header, uint8_t hlen, const uint8_t *body, uint8_t blen)
{
    // Serial.println("writecommand()");
    command = header[0];
    writeFrame(header, hlen, body, blen);

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
    // Serial.println("isready()");
    digitalWrite(_ss, LOW);
    write(STATUS_READ);
    uint8_t status = read() & 1;
    digitalWrite(_ss, HIGH);
    return status;
}

int8_t readAckFrame()
{
    // Serial.println("readACKFrame()");
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

void init_pipeline(uint8_t _pin);



////////////////////////////////////////////////////////////////




// bool PN532::readPassiveTargetID(uint8_t cardbaudrate, uint8_t *uid, uint8_t *uidLength, uint16_t timeout, bool inlist)
// {
//     pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
//     pn532_packetbuffer[1] = 1;  // max 1 cards at once (we can set this to 2 later)
//     pn532_packetbuffer[2] = cardbaudrate;

//     if (writeCommand(pn532_packetbuffer, 3)) {
//         return 0x0;  // command failed
//     }

//     // read data packet
//     if (readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer), timeout) < 0) {
//         return 0x0;
//     }

//     if (pn532_packetbuffer[0] != 1)
//         return 0;

//     uint16_t sens_res = pn532_packetbuffer[2];
//     sens_res <<= 8;
//     sens_res |= pn532_packetbuffer[3];

//     /* Card appears to be Mifare Classic */
//     *uidLength = pn532_packetbuffer[5];

//     for (uint8_t i = 0; i < pn532_packetbuffer[5]; i++) {
//         uid[i] = pn532_packetbuffer[6 + i];
//     }

//     if (inlist) {
//         inListedTag = pn532_packetbuffer[1];
//     }

//     return 1;
// }




// ////////////////////////////////////////////////////////////////////////////////////////////




// mifareclassic_AuthenticateBlock (uint8_t *uid, uint8_t uidLen, uint32_t blockNumber, uint8_t keyNumber, uint8_t *keyData)
// {
//     uint8_t i;

//     // Hang on to the key and uid data
//     memcpy (_key, keyData, 6);
//     memcpy (_uid, uid, uidLen);
//     _uidLen = uidLen;

//     // Prepare the authentication command //
//     pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;   /* Data Exchange Header */
//     pn532_packetbuffer[1] = 1;                              /* Max card numbers */
//     pn532_packetbuffer[2] = (keyNumber) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
//     pn532_packetbuffer[3] = blockNumber;                    /* Block Number (1K = 0..63, 4K = 0..255 */
//     memcpy (pn532_packetbuffer + 4, _key, 6);
//     for (i = 0; i < _uidLen; i++) {
//         pn532_packetbuffer[10 + i] = _uid[i];              /* 4 bytes card ID */
//     }

//     if (writeCommand(pn532_packetbuffer, 10 + _uidLen))
//         return 0;

//     // Read the response packet
//     readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer));

//     // Check if the response is valid and we are authenticated???
//     // for an auth success it should be bytes 5-7: 0xD5 0x41 0x00
//     // Mifare auth error is technically byte 7: 0x14 but anything other and 0x00 is not good
//     if (pn532_packetbuffer[0] != 0x00) {
//         return 0;
//     }

//     return 1;
// }




// ////////////////////////////////////////////////////////////////////////////////////////////////////////




// mifareclassic_ReadDataBlock (uint8_t blockNumber, uint8_t *data)
// {

//     /* Prepare the command */
//     pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
//     pn532_packetbuffer[1] = 1;                      /* Card number */
//     pn532_packetbuffer[2] = MIFARE_CMD_READ;        /* Mifare Read command = 0x30 */
//     pn532_packetbuffer[3] = blockNumber;            /* Block Number (0..63 for 1K, 0..255 for 4K) */

//     /* Send the command */
//     if (writeCommand(pn532_packetbuffer, 4)) {
//         return 0;
//     }

//     /* Read the response packet */
//     readResponse(pn532_packetbuffer, sizeof(pn532_packetbuffer));

//     /* If byte 8 isn't 0x00 we probably have an error */
//     if (pn532_packetbuffer[0] != 0x00) {
//         return 0;
//     }

//     /* Copy the 16 data bytes to the output buffer        */
//     /* Block content starts at byte 9 of a valid response */
//     memcpy (data, pn532_packetbuffer + 1, 16);

//     return 1;
// }