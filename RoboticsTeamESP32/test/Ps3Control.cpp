#include <Ps3Controller.h>

bool isConnected = true;

void setup()
{
    Serial.begin(115200);
  Ps3.begin("2c:81:58:49:e4:01");
    Serial.println("Ready.");
}

void loop()
{
  if (Ps3.isConnected() && isConnected){
    Serial.println("Connected!");
    isConnected = false;
  }

  if(Ps3.data.button.cross){
    Serial.println("X Button");
  }

  if(Ps3.data.button.square){
    Serial.printf("Square Button");
  }
}