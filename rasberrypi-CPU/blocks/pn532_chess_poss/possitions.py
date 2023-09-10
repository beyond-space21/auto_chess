import pn532
from smbus import SMBus
from time import sleep
from threading import Thread

CS   = 8
MOSI = 10
MISO = 9
SCLK = 11

addr = 0x8
bus = SMBus(1)
  
pn532 = pn532.PN532(cs=CS, sclk=SCLK, mosi=MOSI, miso=MISO)
pn532.begin()
pn532.SAM_configuration()

buff_len = 1

buff=['?']*buff_len

on = 129
off = 128

def read_sqr(index):
    uid = pn532.read_passive_target()
    if uid is not None:
        data = pn532.mifare_classic_read_block(5)
        if data is None:
            print('index: ',index,' Failed to read block!')
            return ''
        st=''
        for i in data:
            st = st + ' ' + str(i)
        return chr(data[10])
    return ''

def thread_func():
    while True:
        for index in range(buff_len):
            bus.write_byte(addr,index)
            buff[index] = read_sqr(index) 
            bus.write_byte(addr,off)
            sleep(0.1)

process = Thread(target=thread_func)