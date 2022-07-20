import cv2
import pickle


face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_alt2.xml')
recognizer = cv2.face.LBPHFaceRecognizer_create()
recognizer.read("./mytrainer.xml")

labels = {}

with open("label.pickle","rb") as f:
    origin_labels = pickle.load(f)
    labels = {v:k for k, v in origin_labels.items()}   #To turn value & key upside down



cap = cv2.VideoCapture(202)#200~299 for linux or 0 for win



while True:
    ret, frame = cap.read()
    if ret:
        gray = cv2.cvtColor(frame,cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3,minNeighbors=5)

        #draw a rectangle on face.
        for (x ,y ,w ,h ) in faces:

            gray_roi = gray[y:y+h, x:x+w]
            id_, conf = recognizer.predict(gray_roi)

            if conf >= 60:
                #print(id_ , conf)

                cv2.rectangle(frame, (x, y), (x + w, y + h), (80, 18, 236), 2)
                cv2.rectangle(frame, (x, y + h - 20), (x + w, y + h), (80, 18, 236), cv2.FILLED)
                cv2.putText(
                    frame,
                    str(labels[id_]),
                    (x + 6, y + h - 6),
                    cv2.FONT_HERSHEY_DUPLEX,
                    0.5,
                    (255, 255, 255),
                    1,
                )





        cv2.imshow("pi_face",frame)

    if (cv2.waitKey(1) & 0xFF == ord("q")) :
        break



cap.release()
cv2.destroyAllWindows()
