#!/usr/bin/env python
# -*- coding: utf-8 -*-
# lsusb to check device name
#dmesg | grep "tty" to find port name

from flask import Flask, request
from flask_cors import cross_origin
import serial,time

app = Flask(__name__)
arduino = serial.Serial('/dev/ttyUSB1',115200,timeout=1)
time.sleep(0.1)

@app.route('/') # root (for testing)
def test():
    return "root"

@app.route('/get', methods = ['GET']) # cellphone app get drawer data
@cross_origin()
def get():
#     return "0,0,0,1,1,1,2,2,0" # return "id1,x1,y1,id2,x2,y2,..."
    boxArr = []
    if arduino.isOpen():
#         print("{} connected!".format(arduino.port))
        receiving = True
        arduino.write(("@").encode())
        while receiving:
            if  arduino.inWaiting()>0:
                answer=arduino.readline().decode()[:-1]
#                 print(answer)
                if answer == "end":
                    receiving = False
                elif answer == "arduino setup":
                    print("arduino setup")
                else:
#                     print(answer)
                    temp = [int(number) for number in answer.split()]
                    boxArr+=temp
    output = ",".join(str(number) for number in boxArr)
    print(output)
    return output

@app.route('/light', methods = ['POST']) # light up drawer #id
@cross_origin()
def light():
    print("light: " + request.values['ids']) # drawer ids: request.values['ids'], return "id1,id2,..."
    lightArr = [int(number) for number in request.values['ids'].split(",")]
#     print(lightArr)
    if arduino.isOpen():
        print(len(lightArr))
        arduino.write(("H").encode())
        arduino.write(len(lightArr).to_bytes(1,byteorder="big"))
        for i in lightArr:
            arduino.write(i.to_bytes(1,byteorder="big"))
        time.sleep(1)
        if  arduino.inWaiting()>0:
            answer=arduino.readline().decode()
            print(answer)
    return "light" # return not important

@app.route('/lock', methods = ['POST']) # lock drawer #id
@cross_origin()
def lock():
    print("lock: " + request.values['id']) # drawer id: request.values['id']
    if arduino.isOpen():
#         print("{} connected!".format(arduino.port))
        arduino.write(("L").encode())
        arduino.write(int(request.values['id']).to_bytes(1,byteorder="big"))
    return "lock" # return not important

app.run(host='0.0.0.0', port=4000) # server run on 'selfIP:4000'