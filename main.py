# -*- coding: utf-8 -*-
from flask import Flask, render_template
from flask_socketio import SocketIO, emit
from twowheel import TwoWheelController
from time import sleep
import serial
from threading import Lock


async_mode = None
thread = None
thread_lock = Lock()
ser = serial.Serial("/dev/ttyAMA0", 115200)
controller = TwoWheelController()
app = Flask(__name__)
socketio =SocketIO(app)

@app.route('/js/nipplejs.min.js')
def send_js():
    return render_template("js/nipplejs.min.js")

@app.route('/')
def index():

    # return app.send_static_file("index.html")
    return render_template('index.html')

@socketio.on("set_axis")
def set_axis(data):
    print("x is {:.2f} y is {:.2f}".format(data['x'],-data['y']))
    # return jsonify(args=args, form=form)
    controller.set_axis(x=data["x"], y=data["y"])
    
@socketio.on('connect')
def test_connect():
    global thread
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(target=background_thread)

def background_thread():
    while True:
        socketio.sleep(2)
        count = ser.inWaiting()
        # print(count)
        if count != 0:
            # 读取内容并回显
            recv = ser.read(count).decode('utf-8')
            # ser.write(recv)
            print(recv)
            socketio.emit('server_response',
                          {
                              'data': recv})
            # Chinese print(recv.decode('gb2312'))

        ser.flushInput()
        sleep(0.1)
       
if __name__ == "__main__":
    #controller = TwoWheelController()
    socketio.run(app, debug=True, host="0.0.0.0")
    ser.close()




