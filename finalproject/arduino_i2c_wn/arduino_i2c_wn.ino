/*  CpE 185 Final Project
  Author: Wesley Nguyen
  Last Modified: May 4, 2018 */

#include <Servo.h>
#include <Wire.h>

//fanServoSlave Address on I2C
#define sAddr 12
Servo servo;

//Pin setups; not touched by Pi
const int fanPinPWM = 3;
const int relayPin = 4;
const int speedValPin = A3;               //Used to allow the 4-pin PWM fan to work
const int servoPin = 9;


//Predefined servo states/fan speeds; not touched by Pi
//Servo values
const int closedVent = 30;
const int partOpenVent = 90;
const int fullOpenVent = 150;
//Fan values
const int offSpeed = 0;
const int slowSpeed = 50;
const int medSpeed = 150;
const int highSpeed = 255;

//Variables passed from Pi
const int toggleVal = 1;
int userInput;
int fanSpeed = 0;

//Variables manipulated only by slave
boolean ventState = false;



void setup()
{
  pinMode(relayPin, OUTPUT);
  pinMode(speedValPin, INPUT);
  digitalWrite(relayPin, HIGH);
  servo.attach(servoPin);
  Wire.begin(sAddr);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
}


void loop()
{
  fanPWM();
  ventControl();
  delay(10);
}


void receiveData(int byteCount)
{
  while(Wire.available())
  {
    userInput = Wire.read();
    if (userInput == toggleVal)
    {
      digitalWrite(relayPin, HIGH);
      ventState = 0;
    }
    else
    {
      fanSpeed = userInput-1;
      digitalWrite(relayPin, LOW);
      ventState = 1;
    }
  }
}


//Send fanSpeed back to the Pi; messages are sent as int, range 1-4
void sendData()
{
  switch (fanSpeed)
  {
    case 0:
      Wire.write(0);
      break;
    case 1:
      Wire.write(1);
      break;
    case 2:
      Wire.write(2);
      break;
    default:
      Wire.write(3);
  }
}


//Manipulates the fanspeed
void fanPWM()
{
  switch (fanSpeed)
  {
    case 0:
      analogWrite(fanPinPWM, offSpeed);

      break;
    case 1:
      analogWrite(fanPinPWM, slowSpeed);

      break;
    case 2:
      analogWrite(fanPinPWM, medSpeed);

      break;
    default:
      analogWrite(fanPinPWM, highSpeed);

  }
}


//Manipulates the servo controlling the vent; is closed when the fan is not in operation
void ventControl()
{
  if (ventState == 1)
  {
    if (fanSpeed == 1 || fanSpeed == 2)
      servo.write(partOpenVent);
    else if (fanSpeed == 3)
      servo.write(fullOpenVent);
  }
  else
  {
    servo.write(closedVent);
  }
    
}

