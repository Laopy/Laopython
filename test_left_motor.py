from gpiozero import DistanceSensor
from time import sleep

sensor = DistanceSensor(echo=23, trigger=22)
while True:
    dis = sensor.distance * 100
    print("Distance:{:.1f} ".format(dis))
    sleep(1)
    if  dis<10:
        print("fuck")
    
# -*- coding: utf-8 -*
# -*- coding: utf-8 -*
"""
import serial
import time
# 打开串口
ser = serial.Serial("/dev/ttyAMA0", 115200)
def main():
    while True:
        # 获得接收缓冲区字符
        count = ser.inWaiting()
       # print(count)
        if count != 0:
            # 读取内容并回显
            recv = ser.read(count)
           # ser.write(recv)
            print(recv.decode('utf-8'))
            #print(recv.decode('gb2312'))
        # 清空接收缓冲区
        ser.flushInput()
        # 必要的软件延时
        time.sleep(0.1)
    
if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        if ser != None:
            ser.close()
"""
"""
from gpiozero import LineSensor
from signal import pause

sensor = LineSensor()
sensor.when_line = lambda: print('Line detected')
#sensor.when_no_line = lambda: print('No line detected')
pause()
"""
