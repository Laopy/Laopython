import re
from pi_detection import detect_fire
from simple_pid import PID
import os
import cv2
from flask_socketio import SocketIO, emit
from flask import Flask, render_template, Response
from time import sleep
from matplotlib import pyplot as plt
from threading import Lock
from multiprocessing import Process ,Queue,Manager
from gpiozero import DistanceSensor
import serial
from twowheel import TwoWheelController
from device import Motor



thread = None
thread_lock = Lock()

if os.environ.get('WERKZEUG_RUN_MAIN'):
    cap = cv2.VideoCapture(0)


app = Flask(__name__)
socketio =SocketIO(app,async_mode='threading')

controller = TwoWheelController()
pid = PID(0.6, 0, 0.45, setpoint=0)
sensor = DistanceSensor(echo=23, trigger=22)
ser = serial.Serial("/dev/ttyAMA0", 115200)

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
            que.put(fire)
            # pipe.send(img_q)
            #cv2.imshow("show", fire[1])


        
    cap.release()

def gen_frames(que):
    while 1:
        face_center_ndc =que.get()
        ret, buffer = cv2.imencode('.jpg', face_center_ndc[1])
        face_center_ndc[1] = buffer.tobytes()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + face_center_ndc[1] + b'\r\n')  # concat frame one by one and show result

        in_ = face_center_ndc[0] if (face_center_ndc is not None) else 0.0

        if face_center_ndc is None:
            go = 0.0
        if (face_center_ndc is not None) and (-0.4 < in_ < 0.4):
            go = 0.6
            in_ = 0.0
        out = pid(in_)
        if (out > 0.5):
            out = 0.5
        if (out < -0.5):
            out = -0.5
#        print("in=%.2f  out=%.2f" % (in_, out))
        dis=sensor.distance * 100
        if dis<20:
            controller.set_axis(x=-0.5)
            sleep(0.5)
        controller.set_axis(x=-out)
    #show_plt(out, pid)
    #p1.terminate()
    #ser.close()


@app.route('/video_feed')
def video_feed():
    cap.set(cv2.CAP_PROP_FPS, 10) 
    manager = Manager()
    que = manager.Queue()
    p1 = Process(target=test_f, args=(que, cap,))
    #p2 = Process(target=test_f, args=(que, cap,))
    p1.start()
    #p2.start()
    # p1.join()
    #Video streaming route. Put this in the src attribute of an img tag
    return Response(gen_frames(que), mimetype='multipart/x-mixed-replace; boundary=frame')



@app.route('/js/nipplejs.min.js')
def send_js():
    return render_template("js/nipplejs.min.js")


@app.route('/')
def index():
    # return app.send_static_file("index.html")
    return render_template('index.html')


@socketio.on("set_axis")
def set_axis(data):

    print("x is {:.2f} y is {:.2f}".format(data['x'], -data['y']))
    # return jsonify(args=args, form=form)
    controller.set_axis(x=data["x"], y=-data["y"])



@socketio.on('connect')
def test_connect():
    global thread
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(target=background_thread)


def background_thread():
    while True:
        socketio.sleep(3)
        count = ser.inWaiting()
        # print(count)
        if count != 0 :
            # 读取内容并回显
            recv = ser.read(count).decode('utf-8')
            # ser.write(recv)
            print(recv)
            mes = re.match('<Temp:(.*\d+.\d+)>,<PPM:(\d+.\d+)>', recv, flags=0)
            if recv ==None:
                break
            elif re.search('<fire:(\w)>', recv, flags=0):
              
                socketio.emit('server_response',
                          {
                              'data': 'fire warning!!!'})
            elif re.search('<smoke:alarm>', recv, flags=0):
                
                socketio.emit('server_response',
                          {
                              'data': 'smoke warning!!!'})
          
            else:
                print(recv)
                
                if mes !=None:
                    data = "The tempture is {}℃ and The PPM is {}%".format(mes.group(1),mes.group(2))
                    socketio.emit('server_response',
                          {
                              'data': data})
                    

            # Chinese print(recv.decode('gb2312'))

        ser.flushInput()
        sleep(0.5)

"""
def show_plt(out,pid):
    # setting time index dt
    global start_time, last_time, setpoint, y, x
    current_time = time.time()
    dt = (current_time - last_time)

    x += [current_time - start_time]
    y += [out]
    setpoint += [pid.setpoint]

    last_time = current_time

    # Only rotate the robot without moving forward and backward (y is set to 0).
    # sio.emit("set_axis", {"x": -out, "y": 0.0})
    if (cv2.waitKey(1) & 0xFF == ord("q")):
        # output visualization from plt
        plt.plot(x, setpoint, label='target')
        plt.plot(x, y, label='PID')
        plt.xlabel('time')
        plt.ylabel('outcome')
        plt.legend()

        plt.savefig("/home/laopy/Laopython/pi/rpi-robot/images/show.png")  # save in destination
        plt.show()
        exit(0)
"""

if __name__ == '__main__':
    socketio.run(app, debug=True, host="0.0.0.0",port=5000)
    #ser.close()
'''
def recv():
    while True:
        # 获得接收缓冲区字符
        count = ser.inWaiting()
        # print(count)
        if count != 0:
            # 读取内容并回显
            recv = ser.read(count)
            # ser.write(recv)
            print(recv.decode('utf-8'))
            yield recv.decode('utf-8')
            #Chinese print(recv.decode('gb2312'))
        # 清空接收缓冲区
        ser.flushInput()
        # 必要的软件延时
        time.sleep(0.1)
'''




"""
from gpiozero import DistanceSensor
from time import sleep

sensor = DistanceSensor(echo=24, trigger=23)
while True:
    print("Distance:{:.1f} ".format(sensor.distance * 100))
    sleep(1)tim = re.findall('<Temp:(\d+.\d+)>,<PPM:(\d+.\d+)>',tom)
"""
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
            #Chinese print(recv.decode('gb2312'))
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

