#include <Arduino.h>
#include <ESP32Servo.h>
#include "motorControl.hpp"


void motorControl::runAtSetup(){
        
    //attach pins to motors and set min and max pwm freq
    motor1.attach(motor1Pin, minUs, maxUs);
	motor2.attach(motor2Pin, minUs, maxUs);
	motor3.attach(motor3Pin, minUs, maxUs);
	motor4.attach(motor4Pin, minUs, maxUs);
	servo1.attach(servo1Pin, minUs, maxUs);
    motor5.attach(motor1Pin, minUs, maxUs);
    motor6.attach(motor1Pin, minUs, maxUs);

        
    // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
        

    }

    void motorControl::forward(int analogVar){

        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteFw);
        motor2.writeMicroseconds(pwmWriteFw);
        motor3.writeMicroseconds(pwmWriteFw);
        motor4.writeMicroseconds(pwmWriteFw);

        Serial.print(pwmWriteFw);
        Serial.println("Forward");

    };

    void motorControl::backward(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteBk);
        motor2.writeMicroseconds(pwmWriteBk);
        motor3.writeMicroseconds(pwmWriteBk);
        motor4.writeMicroseconds(pwmWriteBk);

    };

    void motorControl::rightShift(int analogVar){
        
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteFw);
        motor2.writeMicroseconds(pwmWriteBk);
        motor3.writeMicroseconds(pwmWriteBk);
        motor4.writeMicroseconds(pwmWriteFw);
    }

    void motorControl::leftShift(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteBk);
        motor2.writeMicroseconds(pwmWriteFw);
        motor3.writeMicroseconds(pwmWriteFw);
        motor4.writeMicroseconds(pwmWriteBk);
    }

    void motorControl::CW(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteFw);
        motor2.writeMicroseconds(pwmWriteBk);
        motor3.writeMicroseconds(pwmWriteFw);
        motor4.writeMicroseconds(pwmWriteBk);
    }

    void motorControl::CCW(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteBk);
        motor2.writeMicroseconds(pwmWriteFw);
        motor3.writeMicroseconds(pwmWriteBk);
        motor4.writeMicroseconds(pwmWriteFw);
    }