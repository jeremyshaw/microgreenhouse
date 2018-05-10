#!/usr/bin/python3
import _thread, smbus, socket, sys, time

#still needs: more try:except error escapes to prevent crashes on I/O errors

bus = smbus.SMBus(1)
tf = 'plantdata.txt'

address = 12 #default to prevent potential crash
waterlevel = 0
waterpump = 0
heaterswitch = 0
lightswitch = 0
lowtemp = 0
hitemp = 0
fanswitch = 0
temp_sensor = 0
light_sensor = 0
humidity_sensor = 0

def web_server():
    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSocket.bind(('', 12341))
    serverSocket.listen(1)

    while True:
        connectionSocket, addr = serverSocket.accept()
        print('Ready to serve...')
        message = connectionSocket.recv(1024)
        try:
            bus.write_byte(11, 1)
            waterlevel = bus.read_byte(11)
        except:
            waterlevel = 'error'
        try:
            bus.write_byte(13, 1)
            temp_sensor = bus.read_byte(13)
        except:
            temp_sensor = 'error'
        try:
            bus.write_byte(13, 2)
            light_sensor = bus.read_byte(13)
        except:
            light_sensor = 'error'
        try:
            bus.write_byte(13, 3)
            humidity_sensor = bus.read_byte(13)
        except:
            humidity_sensor = 'error'
        connectionSocket.send(b'HTTP/1.1 200 OK\nContent-Type: text/html\n\n')
        connectionSocket.send(b'<html><head><title>Micro Greenhouse</title></head><body>')
        connectionSocket.send(b'waterlevel = ' + bytes(str(waterlevel), 'ascii') + b'<p>')
        connectionSocket.send(b'temp sensor = ' + bytes(str(temp_sensor), 'ascii') + b'C <p>')
        connectionSocket.send(b'light sensor = ' + bytes(str(light_sensor), 'ascii') + b'<p>')
        connectionSocket.send(b'humidity sensor = ' + bytes(str(humidity_sensor), 'ascii') + b'% <p>')
        connectionSocket.send(b'</body></html>')
        connectionSocket.close()
        
try: 
    _thread.start_new_thread(web_server,() )
except:
    print ("Error: unable to start server thread")

def writeNumber(num):
    bus.write_byte(address, num)
    return -1

def readNumber():
    num = bus.read_byte(address)
    return num

def number11():
    #jeremy
    print("This is Jeremy's Arduino")
    while True:
        print("0 for exit")
        print("1 for water level")
        print("2 for water pump on/off")
        print("3 for heater on/off")
        print("4 for light switch on/off")
        try:
            options = int(input("Enter 0-4: "))
        except Exception:
            print("input invalid, try again")
            options = 9
        if options == 0:
            break
        elif options == 1:
            writeNumber(options)
            time.sleep(1)
            waterlevel = readNumber()
            print("water level = " + str(waterlevel))
        elif options == 2:
            writeNumber(options)
            time.sleep(1)
            waterpump = (readNumber() + 1) % 2
            print("water pump state = " + str(waterpump))
        elif options == 3:
            writeNumber(options)
            time.sleep(1)
            heaterswitch = (readNumber() + 1 ) % 2
            print("heater state = " + str(heaterswitch))
        elif options == 4:
            writeNumber(options)
            time.sleep(1)
            lightswitch = readNumber()
            print("light state = " + str(lightswitch))
        if not options:
            continue


def number12():
    #wesley
    print("you chose Wesley's Arduino")
    while True:
        #print("Current temp is <insert temp>")
        print("0 for exit")
        print("1 to turn off fan")
        print("2 to set low fan speed")
        print("3 to set medium fan speed")
        print("4 to set high fan speed")
        try:
            options = int(input("Enter 0-4: "))
        except Exception:
            print("input invalid, try again")
            options = 9
        if options == 0:
            break
        elif options == 1:
            writeNumber(options)
            time.sleep(1)
        elif options == 2:
            writeNumber(options)
            time.sleep(1)
        elif options == 3:
            writeNumber(options)
            time.sleep(1)
        elif options == 4:
            writeNumber(options)
            time.sleep(1)
        if not options:
            continue

        fanswitch = readNumber()
        print("fan status: " + str(fanswitch))
        
def number13():
    #daniel
    print("Daniel's Arduino reporting")
    while True:
        print("0 for exit")
        print("1 for temperature sensor")
        print("2 for light sensor")
        print("3 for humidity sensor")
        try:
            options = int(input("Enter 0-3: "))
        except Exception:
            print("input invalid, try again")
            options = 9
        if options == 0:
            break
        elif options == 1:
            writeNumber(options)
            time.sleep(1)
            temp_sensor = readNumber()
            print("temperature = " + str(temp_sensor))
        elif options == 2:
            writeNumber(options)
            time.sleep(1)
            light_sensor = readNumber()
            print("light = " + str(light_sensor))
        elif options == 3:
            writeNumber(options)
            time.sleep(1)
            humidity_sensor = readNumber()
            print("humidity = " + str(humidity_sensor))
        if not options:
            continue

def writeFile():
    f = open(tf, 'w')
    f.writelines(str(waterlevel) +"\n" + str(waterpump) + "\n"
            + str(heaterswitch) + "\n" + str(lightswitch) + "\n"
            + str(lowtemp) + "\n" + str(hitemp) + "\n"
            + str(fanswitch) + "\n" + str(temp_sensor) + "\n"
            + str(light_sensor) + "\n" + str(humidity_sensor) + "\n"
            )
    f.close()

def showData():
    print("water level = " + str(waterlevel))
    print("water pump = " + str(waterpump))

while True:
    print("0 for write file")
    print("1 for show data")
    print("11 for Jeremy")
    print("12 for Wesley")
    print("13 for Daniel")
    try:
        address = int(input("Device address: \n"))
    except Exception:
        print("invalid input, try again.")
    if address == 11:
        number11()
    elif address == 12:
        number12()
    elif address == 13:
        number13()
    elif address == 1:
        showData()
    elif address == 0:
        writeFile()
    else:
        print("Incorrect entry, \'" + str(address) + "\' not supported")

