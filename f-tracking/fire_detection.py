from gpiozero import DistanceSensor
import serial
import cv2
import numpy as np
from twowheel import TwoWheelController
from multiprocessing import Process ,Queue,Manager
from simple_pid import PID
from pi_detection import detect_fire
from time import sleep
import re

def test_f(que,cap):
    while 1:
        ret,img = cap.read()
        if ret:
            fire = detect_fire(img)
            lenth = que.qsize()
            #print("que lenth: ", lenth)

            if lenth > 2:
                for i in range(lenth - 2):
                    img = que.get()  # 清除缓存
            que.put(fire[0])
            # pipe.send(img_q)
            #cv2.imshow("show", fire[1])



if __name__ == '__main__':
   
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FPS, 12)
    controller = TwoWheelController()
    pid = PID(0.55, 0, 0.3, setpoint=0)
    sensor = DistanceSensor(echo=23, trigger=22)
    ser = serial.Serial("/dev/ttyAMA0", 115200) 
    manager = Manager()
    que = manager.Queue()
    p1 = Process(target=test_f, args=(que, cap,))
    p1.start()
    print("processing Alreadly!")
    # p1.join()
    while 1:
        count = ser.inWaiting()
        # print(count)
        if count != 0 :
            #     ^o^v ^f^e       ^{^~ ^x 
            recv = ser.read(count).decode('utf-8')
            # ser.write(recv)
            print(recv)
            recv = re.search('<fire:(\w)>', recv, flags=0).group(1)
        if recv=='F':
            controller.set_axis()
            sleep(0.3)
        elif recv=='L':
            controller.set_axis(x=-0.5)
            sleep(0.3)
        elif recv=='R':
            controller.set_axis(x=0.5)
            sleep(0.3)

        dis=sensor.distance * 100
        if dis<20:
            controller.set_axis(x=-0.5)
            sleep(0.5)
        in_ = que.get()
        if (-0.4 < in_ < 0.4):
            go = 0.6
            in_ = 0.0
        out = pid(in_)
        if (out > 0.5):
            out = 0.5
        if (out < -0.5):
            out = -0.5
        print(in_, -out)
        controller.set_axis(x=-out, y=go)
       
        

