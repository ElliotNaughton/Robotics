#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>

Servo motor1; //front left motor
Servo motor2; //front right motor
Servo motor3; //back left motor
Servo motor4; //back right motor
Servo servo1; //close open plow motor
Servo motor5; //motor for hanging right
Servo motor6; //motor for hanging left

int minUs = 1050; //minumum PWM
int midUs = 1500; //mid range PWN
int maxUs = 1950; //maximum PWM

int motor1Pin = 11;
int motor2Pin = 12;
int motor3Pin = 13;
int motor4Pin = 14;
int servo1Pin = 4;

ESP32PWM pwm;

int ps3MaxAnalogNeg = -130;
int ps3MaxAnalogPos = 130;
int zero = 0;
int ps3StickRead;
int pwmWrite;

void setup() {
	
  Serial.begin(115200);

  Ps3.begin("2c:81:58:49:e4:01");
  Serial.println("Ready.");




}

void loop()
{

  if (Ps3.isConnected()){
    Serial.println("Connected!");
  }

    //Horizontal Movement of the Left Stick
  if(Ps3.event.analog_changed.stick.lx){
      
    if(Ps3.data.analog.stick.lx > 0){
      rightShift(Ps3.data.analog.stick.lx);
    }

    if(Ps3.data.analog.stick.lx < 0){
      leftShift(Ps3.data.analog.stick.lx);
    }
    
  }

    //Vertical movement of the Left sick
  if(Ps3.event.analog_changed.stick.ly){
    ps3StickRead = Ps3.data.analog.stick.ly;
    
    if(Ps3.data.analog.stick.ly > 0){
      forward(Ps3.data.analog.stick.ly);
    }

    if(Ps3.data.analog.stick.ly < 0){
      backward(Ps3.data.analog.stick.ly);
    }

  }  

    //Horizontal movement of the Right Stick
  if(Ps3.event.analog_changed.stick.rx){
      
    if(Ps3.data.analog.stick.rx > 0){
      rightShift(Ps3.data.analog.stick.rx);
    }

    if(Ps3.data.analog.stick.rx < 0){
      leftShift(Ps3.data.analog.stick.rx);
    }
    
  }  

    //Vertical movement of the Right Stick
  if(Ps3.event.analog_changed.stick.ry){
    ps3StickRead = Ps3.data.analog.stick.ry;
    

  }  





}
