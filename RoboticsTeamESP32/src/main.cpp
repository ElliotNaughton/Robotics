#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>
#include "robotControl.hpp"


robotControl robot;

int ps3StickRead;

void setup() {

  robot.minUs = 1050; //minumum PWM
  robot.midUs = 1500; //mid range PWN
  robot.maxUs = 1950; //maximum PWM

  robot.motor1Pin = 11; //motor 1 pin
  robot.motor2Pin = 12; //motor 2 pin
  robot.motor3Pin = 13; //motor 3 pin
  robot.motor4Pin = 14; //motor 4 pin
  robot.servo1Pin = 4;  //servo 1 pin
  robot.motor5Pin = 39; //climbing motor right side pin
  robot.motor6Pin = 21; //climbing motor left side pin

  robot.ps3MaxAnalogNeg = -130;
  robot.ps3MaxAnalogPos = 130;

  robot.pwmHertz = 50; //Stanard 50hz PWM Freq
	
  Serial.begin(115200);

  Ps3.begin("2c:81:58:49:e4:01");
  Serial.println("Ready.");



  robot.runAtSetup();
}

void loop()
{

  if (Ps3.isConnected()){
    Serial.println("Connected!");
  }

    //Horizontal Movement of the Left Stick
  if(Ps3.event.analog_changed.stick.lx){
      
    if(Ps3.data.analog.stick.lx > 0){
      robot.rightShift(Ps3.data.analog.stick.lx);
    }

    if(Ps3.data.analog.stick.lx < 0){
      robot.leftShift(Ps3.data.analog.stick.lx);
    }
    
  }

    //Vertical movement of the Left sick
  if(Ps3.event.analog_changed.stick.ly){
    ps3StickRead = Ps3.data.analog.stick.ly;
    
    if(Ps3.data.analog.stick.ly > 0){
      robot.forward(Ps3.data.analog.stick.ly);
    }

    if(Ps3.data.analog.stick.ly < 0){
      robot.backward(Ps3.data.analog.stick.ly);
    }

  }  

    //Horizontal movement of the Right Stick
  if(Ps3.event.analog_changed.stick.rx){
      
    if(Ps3.data.analog.stick.rx > 0){
      robot.rightShift(Ps3.data.analog.stick.rx);
    }

    if(Ps3.data.analog.stick.rx < 0){
      robot.leftShift(Ps3.data.analog.stick.rx);
    }
    
  }  

    //Vertical movement of the Right Stick
  if(Ps3.event.analog_changed.stick.ry){
    ps3StickRead = Ps3.data.analog.stick.ry;
    

  }  





}
