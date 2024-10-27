#include <Arduino.h>
#include <ps5Controller.h>


bool isNotConnected = true;

void setup() {
  Serial.begin(115200);


  ps5.begin("bc:c7:46:42:c6:8c"); //replace with MAC address of your controller
  Serial.println("Ready.");
}

void loop() {
  if (ps5.isConnected() && isNotConnected) {
    Serial.println("Connected!");
    isNotConnected = false;
  }

if(ps5.data.button.cross){
  Serial.println("X Button");
}

if(ps5.data.button.square){
  Serial.println("Square Button");
}

Serial.print(ps5.data.analog.stick.lx);

}
