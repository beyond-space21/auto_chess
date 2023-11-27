import time as t

from smbus2 import SMBus
bus = SMBus(1) # indicates /dev/ic2-1

data=[0,0,0,0]

def send(x,y):
  Ndata = [0,0,0,0]
  Ndata[0]=x >> 8
  Ndata[1] = x & 255
  Ndata[2]=y >> 8
  Ndata[3] = y & 255
  bus.write_i2c_block_data(0x8, 0, Ndata)

print('sending a')
send(2000,1300)
t.sleep(9)

print('sending -a')
bus.write_i2c_block_data(0x8, 0, [1])
send(2000,0)
t.sleep(5)

print('sending b')
bus.write_i2c_block_data(0x8, 0, [1])
send(2600,2000)
t.sleep(8)

print('sending -b')
bus.write_i2c_block_data(0x8, 0, [1])
send(4500,0)
t.sleep(9)
bus.write_i2c_block_data(0x8, 0, [1])
print('end')