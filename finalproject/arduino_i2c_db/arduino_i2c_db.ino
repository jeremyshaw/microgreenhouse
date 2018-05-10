#include <Wire.h>
#include "dht.h"
//Jeremy = 11, Wesley = 12, Daniel = 13
#define sAddr 13

int num = 0, renum = 0;

//db
int temp = 0;
int humidity = 0;
int lightLevel = 0;
int lightLevelPin = A0; //3 5 6 9 10 11 on the Pro Mini are PWM pins //but A0-A3 are the ones needed for analog reading, lol
int DHTPowerPin = 3;
//uses DHT11 temp and humidity sensor - fewer wires than seperate temp and hum sensors
#define DHT11_PIN 2
dht DHT;


void setup() {
  //Serial.begin(9600);
  Wire.begin(sAddr);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  pinMode(lightLevelPin, INPUT);
  //this is one way of providing power to the DHT11
  pinMode(DHTPowerPin, OUTPUT);
  digitalWrite(DHTPowerPin, HIGH);
}

void loop() {
  delay(2000); //DHT11 can only update once every 1000ms
  int chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  humidity = DHT.humidity;
  //Serial.println(temp);
  //Serial.println(humidity);
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    num = Wire.read();
  }
}

void sendData() {

  //Option 1 for temperature
  if (num == 1)
  {
    Wire.write(temp);
  }
  //Option 2 for light level reading
  else if (num == 2)
  {
    Wire.write(analogRead(lightLevelPin));
  }
  //Option 3 for humidity
  else if (num == 3)
  {
    Wire.write(humidity);
  }
  //arduino crashes if you ask it for an option that it doesn't know about
  else
  {
    Wire.write(112); //send a number that is out of the range of what the pi is looking for
  }
}
