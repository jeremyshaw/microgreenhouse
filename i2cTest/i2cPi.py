#!/usr/bin/python

import smbus
import time

bus = smbus.SMBus(1)

DEVICE_ADDRESS = 10


def writeNumber(num):
    bus.write_byte(DEVICE_ADDRESS, num)
    return -1

def readNumber():
    num = bus.read_byte(DEVICE_ADDRESS)
    return num

while True:
    DEVICE_ADDRESS = int(input("Enter device address: "))
    var = int(input("Enter single digit: "))
    if not var:
        continue

    writeNumber(var)
    print ("rPi sends: ", var)
    time.sleep(1)

    num = readNumber()
    print("Arduino sends: ", num)
    
