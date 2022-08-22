#from CSDN「https://blog.csdn.net/Zhangrx_/article/details/108796306」
import cv2
import numpy as np

x= y= w= h=0
def detect_fire(frame):
    olderr = np.seterr(all='ignore')#ignore bugw
    #img = cv2.imread('fire/fire4.jpeg')
    redThre = 135  # 115~135 red color threshold
    sThre = 65  # 55~65 saturation threshold
    global x, y, w, h
    while True:

        B = frame[:, :, 0]
        G = frame[:, :, 1]
        R = frame[:, :, 2]
        B1 = frame[:, :, 0] / 255
        G1 = frame[:, :, 1] / 255
        R1 = frame[:, :, 2] / 255
        # print(B ,G ,R  )

        minValue = np.array(
            np.where(R1 <= G1, np.where(G1 <= B1, R1, np.where(R1 <= B1, R1, B1)), np.where(G1 <= B1, G1, B1)))
        sumValue = R1 + G1 + B1
        # formula for S index in HSI

        S = np.array(np.where(sumValue != 0, (1 - 3.0 * minValue / sumValue), 0))
        Sdet = (255 - R) / 20
        SThre = ((255 - R) * sThre / redThre)
         # limit setting
        fireImg = np.array(
            np.where(R > redThre, np.where(R >= G, np.where(G >= B, np.where(S > 0, np.where(S > Sdet, np.where(
                  S >= SThre, 255, 0), 0), 0), 0), 0), 0))

        gray_fireImg = np.zeros([fireImg.shape[0], fireImg.shape[1], 1], np.uint8)
        gray_fireImg[:, :, 0] = fireImg
        meBImg = cv2.medianBlur(gray_fireImg, 5)
        kernel = np.ones((5, 5), np.uint8)
        ProcImg = cv2.dilate(meBImg, kernel)
        # draw img

        contours, _ = cv2.findContours(ProcImg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        # ResImg = frame.copy()
        x = y = w = h = 0
        for c in range(0, len(contours)):
            # top left coordinate(x,y) on rectangle，w(wide)、h(high) on rectangle
            #x, y, w, h = 0
            #global x , y, w, h
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
            y -=240

            #print(w,h)
        return [(x+0.5*w)/320,frame]
        del(frame)

                #print(x, ((x + x + w) * 0.5 / w - 0.5) * 2.0, y, ((y + y + h) * 0.5 / h - 0.5) * 2.0)
                # ((x + x + w) * 0.5 / w - 0.5) * 2.0, ((y + y + h) * 0.5 / h - 0.5) * 2.0


    return None


