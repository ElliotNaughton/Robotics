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

    //setting pwm hz
	motor1.setPeriodHertz(pwmHertz);
	motor2.setPeriodHertz(pwmHertz);
	motor3.setPeriodHertz(pwmHertz);
	motor4.setPeriodHertz(pwmHertz);
	servo1.setPeriodHertz(pwmHertz);
    motor5.setPeriodHertz(pwmHertz);
    motor6.setPeriodHertz(pwmHertz);
        

    }

    void motorControl::forward(int analogVar){

        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //Initialzing Forward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteFw);
        motor2.write(pwmWriteFw);
        motor3.write(pwmWriteFw);
        motor4.write(pwmWriteFw);

    };

    void motorControl::backward(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteBk);
        motor2.write(pwmWriteBk);
        motor3.write(pwmWriteBk);
        motor4.write(pwmWriteBk);

    };

    void motorControl::rightShift(int analogVar){
        
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteFw);
        motor2.write(pwmWriteBk);
        motor3.write(pwmWriteBk);
        motor4.write(pwmWriteFw);
    }

    void motorControl::leftShift(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteBk);
        motor2.write(pwmWriteFw);
        motor3.write(pwmWriteFw);
        motor4.write(pwmWriteBk);
    }

    void motorControl::CW(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteFw);
        motor2.write(pwmWriteBk);
        motor3.write(pwmWriteFw);
        motor4.write(pwmWriteBk);
    }

    void motorControl::CCW(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteBk);
        motor2.write(pwmWriteFw);
        motor3.write(pwmWriteBk);
        motor4.write(pwmWriteFw);
    }