#include <Wire.h>

#define sAddr 11
int num = 0;
int state = 0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  Wire.begin(sAddr);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Start");

}

void loop() {
  delay(100);

}

void receiveData(int byteCount){
  while(Wire.available()){
    num = Wire.read();
    Serial.print("data received: ");
    Serial.println(num);
  }
}

void sendData(){
  int num2 = num + 1;
  Wire.write(num2);
  
}

