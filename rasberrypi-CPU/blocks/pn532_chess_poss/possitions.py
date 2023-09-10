import pn532
from smbus import SMbus
from time import sleep
from threading import Thread

CS   = 8
MOSI = 10
MISO = 9
SCLK = 11

addr = 0x8
bus = SMbus(1)
  
pn532 = pn532.PN532(cs=CS, sclk=SCLK, mosi=MOSI, miso=MISO)
pn532.begin()
pn532.SAM_configuration()


buff=['?']*64

def read_sqr(index):
    bus.write_byte(addr,index)
    sleep(0.1)
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
        for i in range(64):
            buff[i] = read_sqr(i) 

process = Thread(target=thread_func)