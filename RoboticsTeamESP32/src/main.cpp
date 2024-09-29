#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>
#include "motorControl.hpp"
#include "ultraSonic.hpp"


//creating class objects
motorControl robotMove;
ultraSonic sensor;

int ps3StickDeadzone = 4;
int ps3StickRead;

void ps3ReadPwmWrite(void * parameter){
  for(;;){//start infinite loop freeRTOS can break out of loop for other tasks

  if (Ps3.isConnected()){ //test for connection print to serial line
    Serial.println("Connected!");
  }

    //Horizontal Movement of the Left Stick
  if(Ps3.event.analog_changed.stick.lx){
        
      if(Ps3.data.analog.stick.lx > ps3StickDeadzone){//value is larger than deadzone
        robotMove.rightShift(Ps3.data.analog.stick.lx);
      }

      if(Ps3.data.analog.stick.lx < -ps3StickDeadzone){//value is smaller than deadzone
        robotMove.leftShift(Ps3.data.analog.stick.lx);
      }
      
    }

    //Vertical movement of the Left sick
      if(Ps3.event.analog_changed.stick.ly){
        ps3StickRead = Ps3.data.analog.stick.ly;
        
        if(Ps3.data.analog.stick.ly < -ps3StickDeadzone){//value is smaller than deadzone
          robotMove.forward(Ps3.data.analog.stick.ly);
        }

        if(Ps3.data.analog.stick.ly > ps3StickDeadzone){//value is larger than deadzone
          robotMove.backward(Ps3.data.analog.stick.ly);
        }

  }  

    //Horizontal movement of the Right Stick
  if(Ps3.event.analog_changed.stick.rx){
      
    if(Ps3.data.analog.stick.rx > ps3StickDeadzone){//value is larger than deadzone
      robotMove.CW(Ps3.data.analog.stick.rx);
    }

    if(Ps3.data.analog.stick.rx < -ps3StickDeadzone){//value is smaller than deadzone
      robotMove.CCW(Ps3.data.analog.stick.rx);
    }
    
  }  

    //Vertical movement of the Right Stick
    if(Ps3.event.analog_changed.stick.ry){
      ps3StickRead = Ps3.data.analog.stick.ry;
      

  }  
  }
}

void continuosDetection(void * parameter){
  for(;;){ //start infinite loop freeRTOS can break out of loop for other tasks
   
  }
}

void setup() {


/////////////////////////////////////////////////////////////////// robotMove Setup //////////////////////////////////////////////////////

  robotMove.minUs = 1050; //minumum PWM
  robotMove.midUs = 1500; //mid range PWN
  robotMove.maxUs = 1950; //maximum PWM
  robotMove.motor1Pin = 11; //motor 1 pin
  robotMove.motor2Pin = 12; //motor 2 pin
  robotMove.motor3Pin = 13; //motor 3 pin
  robotMove.motor4Pin = 14; //motor 4 pin
  robotMove.servo1Pin = 4;  //servo 1 pin
  robotMove.motor5Pin = 39; //climbing motor right side pin
  robotMove.motor6Pin = 21; //climbing motor left side pin
  robotMove.ps3MaxAnalogNeg = -130; //Ps3 negative analog maximum 
  robotMove.ps3MaxAnalogPos = 130; //Ps3 positive analog maximum
  robotMove.pwmHertz = 50; //Stanard 50hz PWM Freq

/////////////////////////////////////////////////////////////////// sensor setup /////////////////////////////////////////////////////////

  sensor.trigPin1 = 1;
	
  Serial.begin(115200); //starting serial communication for debugging at 115200 baud
  Ps3.begin("2c:81:58:49:e4:01"); //beginning PS3 console emulation at mac adress specified

  //settup functions for classes
  robotMove.runAtSetup();
  sensor.runAtSetup();


/////////////////////////////////////////////////////////////////// free RTOS task creation //////////////////////////////////////////////
 
 xTaskCreate(//task to read from ps3 controller and write to motor pwm
    ps3ReadPwmWrite,    // Function that should be called
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


  Serial.println("Ready."); //print ready to serial consule to know that setup is completed
}

void loop()
{
//empty loop will not run do to use of freeRTOS tasks
}
