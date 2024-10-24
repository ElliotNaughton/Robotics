#include <Ps3Controller.h>
#include <ESP32Servo.h>
#include "ultraSonic.hpp"

bool isNotConnected = true;
Servo Test1;
ultraSonic sensor;

bool objectRumble;

int plowLiftPin = 23; //pin for relay to actuators to lift plow
bool plowState = false; //tracks state of plow, either up (true) or down (false)

int pwmOutput;
int analogVar3;

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo servo1;

int analogVarX;
int analogVarY;
int analogVarF;
int motor1pwm;
int motor2pwm;
int motor3pwm;
int motor4pwm;
int servo1pwm;
int maxUs = 1950;
int midUs = 1500;
int minUs = 1000;
int ps3MaxAnalogPos = 125;
int ps3MaxAnalogNeg = -125;
int ps3Deadzone = 4;
int servo1ClosedPwm = 1000;
int servo1OpenPwm = 1800;

void ps3ReadEsp32Execute(void * parameter); //task to read from ps3 controller and write to motor pwm
void continuosDetection(void * parameter); //task to read and pulse ultrasonic

void setup()
{

  Serial.begin(115200);
  Ps3.begin("2c:81:58:49:e4:01");
  Serial.println("Ready.");

  pinMode(plowLiftPin, OUTPUT);

  motor1.attach(13); //front left motor
  motor2.attach(12); //front right motor
  motor3.attach(33); //back left motor
  motor4.attach(27); //back right motor
  servo1.attach(32);  //close open plow servo

  ultraSonic sensor;
  sensor.normalDuration = 0; //TBD normal duration of ultrasonic pulse at floor distance
  sensor.sensorTrigDelay = 50; //trigger delay in milliseconds, research shows will dissapate in 50 ms
  sensor.sensorDebounce = 1000; //debounce timer in ms for objectDetection in plow

  //trig pin setup
  sensor.trigPin1 = 36;
  sensor.trigPin2 = 39;
  sensor.trigPin3 = 34;
  sensor.trigPin4 = 35;
  sensor.trigPin5 = 33;
  sensor.trigPin6 = 14;
  sensor.trigPin7 = 25;
  sensor.trigPin8 = 26;

  //echo pin setup
  sensor.echoPin1 = 22;
  sensor.echoPin2 = 21;
  sensor.echoPin3 = 19;
  sensor.echoPin4 = 18;
  sensor.echoPin5 = 5;
  sensor.echoPin6 = 17;
  sensor.echoPin7 = 16;
  sensor.echoPin8 = 4;

    xTaskCreate(//task to read from ps3 controller and write to motor pwm
      ps3ReadEsp32Execute,    // Function that should be called
      "PS3 Read PWM Write",  // Name of the task (for debugging)
      10000,            // Stack size (bytes)
      NULL,            // Parameter to pass
      5,               // Task priority
      NULL             // Task handle
  );


  
}

void loop()
{
}

void ps3ReadEsp32Execute(void * parameter){//task to read from ps3 controller and write to motor pwm
  for(;;){//start infinite loop freeRTOS can break out of loop for other tasks
    if (Ps3.isConnected() && isNotConnected){
    Serial.println("Connected!");
    isNotConnected = false;
  }
/*
  Serial.print("lx: ");
  Serial.print(Ps3.data.analog.stick.lx);
  Serial.print(" ly: ");
  Serial.print(Ps3.data.analog.stick.ly);
  Serial.print(" rx: ");
  Serial.print(Ps3.data.analog.stick.rx);
  Serial.print(" ry: ");
  Serial.println(Ps3.data.analog.stick.ry);
*/
  

  if((Ps3.data.analog.stick.rx < -ps3Deadzone)){ 
      analogVarX = abs(Ps3.data.analog.stick.rx);
      
      //Writing pwm signal for motor controllers
      motor1pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor2pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor3pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor4pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
  }

    else if((Ps3.data.analog.stick.rx > ps3Deadzone)){ 
      analogVarX = abs(Ps3.data.analog.stick.rx);
      
      //Writing pwm signal for motor controllers
      motor1pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor2pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor3pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor4pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
  }

  
    //if is in Quadrant 1
  else if((Ps3.data.analog.stick.lx > ps3Deadzone) && (Ps3.data.analog.stick.ly > ps3Deadzone)){
      analogVarX = abs(Ps3.data.analog.stick.lx + 1);
      analogVarY = abs(Ps3.data.analog.stick.ly + 1);

      //Writing pwm signal for motor controllers
      motor1pwm = map(analogVarX - analogVarY , ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs);
      motor2pwm = map(((analogVarX + analogVarY) / 2) , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor3pwm = map(-((analogVarX + analogVarY) / 2) , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor4pwm = map(analogVarY - analogVarX , ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs);
  }

  //in Between 1 and 2
  else if((Ps3.data.analog.stick.ly > ps3Deadzone) && (Ps3.data.analog.stick.lx < ps3Deadzone) && (Ps3.data.analog.stick.lx > -ps3Deadzone)){
      analogVarY = abs(Ps3.data.analog.stick.ly + 1);
      
      //Writing pwm signal for motor controllers
      motor1pwm = map(-analogVarY , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor2pwm = map(analogVarY , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor3pwm = map(-analogVarY , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor4pwm = map(analogVarY , 0, ps3MaxAnalogPos, midUs, maxUs);

  }

  //if is in quadrant 2
  else if((Ps3.data.analog.stick.lx < -ps3Deadzone) && (Ps3.data.analog.stick.ly > ps3Deadzone)){
      analogVarX = abs(Ps3.data.analog.stick.lx);
      analogVarY = abs(Ps3.data.analog.stick.ly + 1);
    
      //Writing pwm signal for motor controllers
      motor1pwm = map(-((analogVarX + analogVarY) / 2) , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor2pwm = map(analogVarY - analogVarX , ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs);
      motor3pwm = map(analogVarX - analogVarY , ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs);
      motor4pwm = map(((analogVarX + analogVarY) / 2) , 0, ps3MaxAnalogPos, midUs, maxUs);
  }

  //in Between 1 and 3 
  else if((Ps3.data.analog.stick.lx > ps3Deadzone) && (Ps3.data.analog.stick.ly < ps3Deadzone) && (Ps3.data.analog.stick.ly > -ps3Deadzone)){
      analogVarX = abs(Ps3.data.analog.stick.lx + 1);
      analogVarY = abs(Ps3.data.analog.stick.ly + 1);
      
      //Writing pwm signal for motor controllers
      motor1pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor2pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor3pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor4pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
  }

    //in Between 2 and 4 
  else if((Ps3.data.analog.stick.lx < -ps3Deadzone) && (Ps3.data.analog.stick.ly < ps3Deadzone) && (Ps3.data.analog.stick.ly > -ps3Deadzone)){
      analogVarX = abs(Ps3.data.analog.stick.lx + 1);
      analogVarY = abs(Ps3.data.analog.stick.ly + 1);
      
      //Writing pwm signal for motor controllers
      motor1pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor2pwm = map(-analogVarX , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor3pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor4pwm = map(analogVarX , 0, ps3MaxAnalogPos, midUs, maxUs);
  }

  //if is in Quadrant 3 
  else if((Ps3.data.analog.stick.lx > ps3Deadzone) && (Ps3.data.analog.stick.ly < -ps3Deadzone)){
      analogVarX = abs(Ps3.data.analog.stick.lx + 1);
      analogVarY = abs(Ps3.data.analog.stick.ly);
      //Writing pwm signal for motor controllers

      motor1pwm = map(((analogVarX + analogVarY) / 2) , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor2pwm = map(analogVarX - analogVarY, ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs);
      motor3pwm = map(analogVarY - analogVarX , ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs);
      motor4pwm = map(-((analogVarX + analogVarY) / 2) , ps3MaxAnalogNeg, 0, minUs, midUs);      
  }

  //in Between 3 and 4
  else if((Ps3.data.analog.stick.ly < -ps3Deadzone) && (Ps3.data.analog.stick.lx < ps3Deadzone) && (Ps3.data.analog.stick.lx > -ps3Deadzone)){
      analogVarX = abs(Ps3.data.analog.stick.lx);
      analogVarY = abs(Ps3.data.analog.stick.ly + 1);

      //Writing pwm signal for motor controllers
      motor1pwm = map(analogVarY , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor2pwm = map(-analogVarY , ps3MaxAnalogNeg, 0, minUs, midUs);
      motor3pwm = map(analogVarY , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor4pwm = map(-analogVarY , ps3MaxAnalogNeg, 0, minUs, midUs);
  }

  //if is in Quadrant 4
  else if((Ps3.data.analog.stick.lx < -ps3Deadzone) && (Ps3.data.analog.stick.ly < -ps3Deadzone)){
      analogVarX = abs(Ps3.data.analog.stick.lx);
      analogVarY = abs(Ps3.data.analog.stick.ly + 1);

      //Writing pwm signal for motor controllers
      motor1pwm = map(analogVarY - analogVarX , ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs);
      motor2pwm = map(-((analogVarX + analogVarY) / 2) , ps3MaxAnalogNeg, 0, minUs, midUs);   
      motor3pwm = map(((analogVarX + analogVarY) / 2) , 0, ps3MaxAnalogPos, midUs, maxUs);
      motor4pwm = map(analogVarX - analogVarY, ps3MaxAnalogNeg, ps3MaxAnalogPos, minUs, maxUs); 
  }

  if (Ps3.event.button_down.r1){
    servo1pwm = servo1OpenPwm;
  }

  else if (Ps3.event.button_down.r2){
    servo1pwm = servo1ClosedPwm;
  }


  else {
      motor1pwm = 1500;
      motor2pwm = 1500;
      motor3pwm = 1500;
      motor4pwm = 1500;
  }

/*
Serial.print("motor1:");
Serial.print(motor1pwm);
Serial.print(" motor2:");
Serial.println(motor2pwm);
Serial.print("motor3:");
Serial.print(motor3pwm);
Serial.print(" motor4:");
Serial.println(motor4pwm);
*/

motor1.writeMicroseconds(motor1pwm);
motor2.writeMicroseconds(motor2pwm);
motor3.writeMicroseconds(motor3pwm);
motor4.writeMicroseconds(motor4pwm);
servo1.writeMicroseconds(servo1pwm);

  }
}