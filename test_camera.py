import cv2
import time


Img_dir = "images/Laopy/"    #"Img_dir" is dir for cv2 imwrite



cap = cv2.VideoCapture(202)#200~299 for linux or 0 for win
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_alt.xml')
#print(face_cascade)

count = 0

while True:
    ret, frame = cap.read()
    if ret:
        gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.5,minNeighbors=5)

        #draw a rectangle onface.
        for (x ,y ,w ,h ) in faces:
            cv2.rectangle(frame,(x,y),(x+w, y+h), (80, 18, 236),2)
            cv2.rectangle(frame, (x, y+h - 20), (x+w, y+h), (80, 18, 236), cv2.FILLED)
            cv2.putText(
                frame,
                "Face",
                (x + 6, y+h - 6),
                cv2.FONT_HERSHEY_DUPLEX,
                0.5,
                (255, 255, 255),
                1,
            )

            #cv2.rectangle(gray, (x, y), (x + w, y + h), (0, 100, 255), 2)
            count += 1
            #cv2.imwrite(Img_dir+ str(count) + '.jpg', gray[y:y+h,x:x+w])
            time.sleep(0.2)


        for (x, y, w, h) in faces:
            print(x, y, w, h,count)
        cv2.imshow("frame",frame)
        cv2.imshow("gray",gray)

    if (cv2.waitKey(1) & 0xFF == ord("q")) or count ==150:
        break



cap.release()
cv2.destroyAllWindows()
