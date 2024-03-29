from device import Motor
from time import sleep
from twowheel import TwoWheelController
#from CSDN「https://blog.csdn.net/Zhangrx_/article/details/108796306」
from simple_pid import PID
import cv2
import numpy as np

olderr = np.seterr(all='ignore')#ignore bugw

cap = cv2.VideoCapture(0) #200~299 for linux or 0 for win
cap.set(cv2.CAP_PROP_FPS, 10)
#img = cv2.imread('fire/fire4.jpeg')
redThre = 115  # 115~135红色分量阈值
sThre = 55  # 55~65饱和度阈值
pid = PID(0.7,0,0.35, setpoint=0)

controller = TwoWheelController()

while True:
    ret, frame = cap.read()
    if ret:
        B = frame[:, :, 0]
        G = frame[:, :, 1]
        R = frame[:, :, 2]
        B1 = frame[:, :, 0] / 255
        G1 = frame[:, :, 1] / 255
        R1 = frame[:, :, 2] / 255
        #print(B ,G ,R  )


        minValue = np.array(
            np.where(R1 <= G1, np.where(G1 <= B1, R1, np.where(R1 <= B1, R1, B1)), np.where(G1 <= B1, G1, B1)))
        sumValue = R1 + G1 + B1
        # HSI中S分量计算公式


        S = np.array(np.where(sumValue != 0, (1 - 3.0 * minValue / sumValue), 0))
        Sdet = (255 - R) / 20
        SThre = ((255 - R) * sThre / redThre)
        #判断条件
        fireImg = np.array(
           np.where(R > redThre, np.where(R >= G, np.where(G >= B, np.where(S > 0, np.where(S > Sdet, np.where(
                S >= SThre, 255, 0), 0), 0), 0), 0), 0))


        gray_fireImg = np.zeros([fireImg.shape[0], fireImg.shape[1], 1], np.uint8)
        gray_fireImg[:, :, 0] = fireImg
        meBImg = cv2.medianBlur(gray_fireImg, 5)
        kernel = np.ones((5, 5), np.uint8)
        ProcImg = cv2.dilate(meBImg, kernel)
        #绘制矩形框

        contours, _ = cv2.findContours(ProcImg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        #ResImg = frame.copy()
        x= y =w = h =0
        for c in range(0, len(contours)):
            # 获取矩形的左上角坐标(x,y)，以及矩形的宽和高w、h
            
            x, y, w, h = cv2.boundingRect(contours[c])
            l_top = (x, y)
            r_bottom = (x + w, y + h)

            cv2.rectangle(frame, (x, y), (x + w, y + h), (80, 18, 236), 2)
            cv2.rectangle(frame, (x, y + h - 20), (x + w, y + h), (80, 18, 236), cv2.FILLED)
            cv2.putText(
                frame,
                "fire",
                (x + 6, y + h - 6),
                cv2.FONT_HERSHEY_DUPLEX,
                0.5,
                (255, 255, 255),
                1, )
            x -=320
            go = 0.0
            in_ = (x+0.5*w)/340
            if  (-0.4<in_<0.4):
                go = 0.6
                in_=0.0
            out = pid(in_)
            if (out>0.65):
                out = 0.55
            if (out<-0.65):
                out = -0.55
            print(in_,-out)
            controller.set_axis(x=-out,y=go)
            x = 0.0
            
        #cv2.imshow("pi_vision",frame)
        #if count == 1 :
            #cv2.imwrite("images/test/"+ str(count) + '.jpg',frame)



cap.release()
cv2.destroyAllWindows()
exit()
