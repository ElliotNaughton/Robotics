#include <ps5Controller.h>

bool isNotConnected = true;

void setup() {
  Serial.begin(115200);

  ps5.begin("BC:C7:46:42:C6:8c"); //replace with MAC address of your controller
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

}
