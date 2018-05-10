#include <Wire.h>

//Jeremy = 11, Wesley = 12, Daniel = 13
#define sAddr 11

int num = 0, renum = 0;

//Jeremy
int relayPump = 3; //active low
int relayPumpState = 1;
int relayHeater = 4; //active low
int relayHeaterState = 1;
int light = 5; //active high?
int lightState = 0;
int waterSense = A0; //reported information
void Jeremy(){
  pinMode(relayPump, OUTPUT);
  pinMode(relayHeater, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(waterSense, INPUT);
  digitalWrite(relayHeater, relayHeaterState);
  digitalWrite(relayPump, relayPumpState);
  digitalWrite(light, lightState);
}

void setup() {
  Wire.begin(sAddr);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Jeremy();
}

void loop() {
  delay(100); 
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    num = Wire.read();
    Serial.print("data received: ");
    Serial.println(num);
  }
}

void sendData() {

  //Option 1 for waterSense
  if (num == 1)
  {
    Wire.write(analogRead(waterSense));
  }
  //Option 2 for relayPump
  else if (num == 2)
  {
    if (relayPumpState == 0) {
      relayPumpState = 1;
    }
    else if (relayPumpState == 1) {
      relayPumpState = 0;
    }
    digitalWrite(relayPump, relayPumpState);
    Wire.write(relayPumpState);
  }
  //Option 3 for relayHeater
  else if (num == 3)
  {
    if (relayHeaterState == 0) {
      relayHeaterState = 1;
    }
    else if (relayHeaterState == 1) {
      relayHeaterState = 0;
    }
    digitalWrite(relayHeater, relayHeaterState);
    Wire.write(relayHeaterState);
  }
  //Option 4 for light
  else if (num == 4)
  {
    if(lightState == 0){
      lightState = 1;
    }
    else if (lightState == 1){
      lightState = 0;
    }
    digitalWrite(light, lightState);
    Wire.write(lightState);
  }
  //arduino crashes if you ask it for an option that it doesn't know about
  else
  {
    Wire.write(111); //send a number that is out of the range of what the pi is looking for
  }
}
