
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

while(1):
  cor = input(">>")
  cor = cor.split(',')
  if len(cor) != 1:
    print('X'+str(cor[0]),'Y'+str(cor[1]))
    send(int(cor[0]),int(cor[1]))
  else:
    print('mag_cmd')
    bus.write_i2c_block_data(0x8, 0, [1])
