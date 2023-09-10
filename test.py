import threading

buff = ['?']*64

from time import sleep



def func():
    v = 0
    while True:
        for i in range(64):
            sleep(0.3)
            buff[i] = v
        v += 1

thr = threading.Thread(target=func)
thr.start()

while True:
    a=input()
    print(buff)