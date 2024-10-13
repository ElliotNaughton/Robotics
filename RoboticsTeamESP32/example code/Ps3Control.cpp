#include <Ps3Controller.h>

void setup()
{
    Serial.begin(115200);
  Ps3.begin("2c:81:58:49:e4:01");
    Serial.println("Ready.");
}

void loop()
{
  if (Ps3.isConnected()){
    Serial.println("Connected!");
  }

  delay(3000);
}