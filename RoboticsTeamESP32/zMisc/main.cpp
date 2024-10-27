#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>
#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "motorControl.hpp"
#include "ultraSonic.hpp"





int ps3StickDeadzone = 4; //what number to start tracking value changes for analog stick reading
int plowLiftPin = 23; //pin for relay to actuators to lift plow
int redLEDpin = 0; //red LED pin
int blueLEDpin = 2; //blue LED pin
int greenLEDpin = 15; //green LED pin

HUSKYLENS huskylens;
SoftwareSerial mySerial(1, 3); // RX, TX
//HUSKYLENS green line >> Pin 10; blue line >> Pin 11
void printResult(HUSKYLENSResult result);




bool plowState = false; //tracks state of plow, either up (true) or down (false)

bool objectRumble = true; //set to false after rumble when object is detected and true when there is no object, ensures controller does not rumble the entire time an object is detected

void ps3ReadEsp32Execute(void * parameter); //task to read from ps3 controller and write to motor pwm
void continuosDetection(void * parameter); //task to read and pulse ultrasonic

void setup() {




/////////////////////////////////////////////////////////////////// robotMove Setup //////////////////////////////////////////////////////
  //creating class objects
  motorControl robotMove;
  robotMove.minUs = 1050; //minumum PWM
  robotMove.midUs = 1500; //mid range PWN
  robotMove.maxUs = 1950; //maximum PWM
  //motor pin setup
  robotMove.motor1Pin = 13; //front left motor
  robotMove.motor2Pin = 12; //front right motor
  robotMove.motor3Pin = 14; //back left motor
  robotMove.motor4Pin = 27; //back right motor
  robotMove.servo1Pin = 15;  //close open plow servo
  robotMove.ps3MaxAnalogNeg = -130; //Ps3 negative analog maximum 
  robotMove.ps3MaxAnalogPos = 130; //Ps3 positive analog maximum
  robotMove.pwmHertz = 50; //Standard 50hz PWM Freq
  robotMove.runAtSetup();//initialize and tie above setup to hardware

/////////////////////////////////////////////////////////////////// ultrasonic sensor setup //////////////////////////////////////////////

  ultraSonic sensor;
  sensor.normalDuration = 0; //TBD normal duration of ultrasonic pulse at floor distance
  sensor.sensorTrigDelay = 50; //trigger delay in milliseconds, research shows will dissapate in 50 ms
  sensor.sensorDebounce = 1000; //debounce timer in ms for objectDetection in plow

  //trig pin setup
  sensor.trigPin1 = 36;
  sensor.trigPin2 = 39;
  sensor.trigPin3 = 34;
  sensor.trigPin4 = 35;
  sensor.trigPin5 = 32;
  sensor.trigPin6 = 33;
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

  //sensor.runAtSetup();//initialize and tie above setup to hardware

/////////////////////////////////////////////////////////////////// Miscellaneous Setup //////////////////////////////////////////////////
	
  Serial.begin(115200); //starting serial communication for debugging at 115200 baud
  Ps3.begin("2c:81:58:49:e4:01"); //beginning PS3 console emulation at mac adress specified
  pinMode(plowLiftPin ,OUTPUT); //plow lift relay pin set up

/////////////////////////////////////////////////////////////////// free RTOS task creation //////////////////////////////////////////////
  
  xTaskCreate(//task to read from ps3 controller and write to motor pwm
      ps3ReadEsp32Execute,    // Function that should be called
      "PS3 Read PWM Write",  // Name of the task (for debugging)
      10000,            // Stack size (bytes)
      NULL,            // Parameter to pass
      5,               // Task priority
      NULL             // Task handle
  );

  xTaskCreate(//task to read and pulse ultrasonic
      continuosDetection,    // Function that should be called
      "Continuous Detection",  // Name of the task (for debugging)
      10000,            // Stack size (bytes)
      NULL,            // Parameter to pass
      4,               // Task priority
      NULL             // Task handle
  );

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Ready."); //print ready to serial consule to know that setup is completed
}

/////////////////////////////////////////////////////////////////// Start of repeating functions /////////////////////////////////////////

void loop()
{
//empty loop will not run do to use of freeRTOS tasks
}

void ps3ReadEsp32Execute(void * parameter){//task to read from ps3 controller and write to motor pwm
  for(;;){//start infinite loop freeRTOS can break out of loop for other tasks

  if (Ps3.isConnected()){ //test for connection print to serial line
    Serial.println("Connected!");
  }

  //Horizontal Movement of the Left Stick
  if(Ps3.event.analog_changed.stick.lx){

      if(Ps3.data.analog.stick.lx > ps3StickDeadzone){robotMove.rightShift(Ps3.data.analog.stick.lx);}

      if(Ps3.data.analog.stick.lx < -ps3StickDeadzone){robotMove.leftShift(Ps3.data.analog.stick.lx);}
  }

  //Vertical movement of the Left sick
  if(Ps3.event.analog_changed.stick.ly){
    
    if(Ps3.data.analog.stick.ly < -ps3StickDeadzone){robotMove.forward(Ps3.data.analog.stick.ly);}

    if(Ps3.data.analog.stick.ly > ps3StickDeadzone){robotMove.backward(Ps3.data.analog.stick.ly);}
  }  

  //Horizontal movement of the Right Stick
  if(Ps3.event.analog_changed.stick.rx){

    if(Ps3.data.analog.stick.rx > ps3StickDeadzone){robotMove.CW(Ps3.data.analog.stick.rx);}

    if(Ps3.data.analog.stick.rx < -ps3StickDeadzone){robotMove.CCW(Ps3.data.analog.stick.rx);} 
  }  

  //Vertical movement of the Right Stick
  if(Ps3.event.analog_changed.stick.ry){


    }  
  }

  //right bumper
  if(Ps3.event.button_down.r1){ 
    if(plowState) {digitalWrite(plowLiftPin,LOW); plowState = false;}//toggle plow up and down
    else {digitalWrite(plowLiftPin,HIGH); plowState = true;}


}
}

void continuosDetection(void * parameter){//task to read and pulse ultrasonic
  for(;;){ //start infinite loop freeRTOS can break out of loop for other tasks
    sensor.currentMillis = millis();
 
    if(sensor.objectDetected()){
      Ps3.setPlayer(4);

      if (objectRumble){Ps3.setRumble(50,1);}
      objectRumble = false;
    }
    else
      Ps3.setPlayer(1);
      objectRumble = true;
  }
}
