#include <Arduino.h>
#include <ESP32Servo.h>
#include "robotControl.h"

class robot{
    private:

    Servo motor1; //front left motor
    Servo motor2; //front right motor
    Servo motor3; //back left motor
    Servo motor4; //back right motor
    Servo servo1; //close open plow motor
    Servo motor5; //motor for hanging | right side
    Servo motor6; //motor for hanging | left side

    public: 
        
    int minUs;//minumum PWM
    int midUs;//mid range PWM
    int maxUs;//maximum PWM

    //Ps3 Controller maximum and miniumum position values
    int ps3MaxAnalogNeg;
    int ps3MaxAnalogPos;

    //pins which the motors will recieve PWM signals from
    int motor1Pin;
    int motor2Pin;
    int motor3Pin;
    int motor4Pin;
    int servo1Pin;
    int motor5Pin;
    int motor6Pin;

    int pwmHertz;//Freq of the PWM signal



    void runAtSetup(){
        
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
	motor1.setPeriodHertz(50);      // Standard 50hz
	motor2.setPeriodHertz(50);      // Standard 50hz
	motor3.setPeriodHertz(50);      // Standard 50hz
	motor4.setPeriodHertz(50);      // Standard 50hz
	servo1.setPeriodHertz(50);      // Standard 50hz
    motor5.setPeriodHertz(50);      // Standard 50hz
    motor6.setPeriodHertz(50);      // Standard 50hz
        

    }

    void forward(int analogVar){

        abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //Initialzing Forward
        int pwmWriteBk;  //and backward Analog variables

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(analogVar * -1, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteFw);
        motor2.write(pwmWriteFw);
        motor3.write(pwmWriteFw);
        motor4.write(pwmWriteFw);

    };

    void backward(int analogVar){
    
        abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(analogVar * -1, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteBk);
        motor2.write(pwmWriteBk);
        motor3.write(pwmWriteBk);
        motor4.write(pwmWriteBk);

    };

    void rightShift(int analogVar){
        
        abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(analogVar * -1, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteFw);
        motor2.write(pwmWriteBk);
        motor3.write(pwmWriteBk);
        motor4.write(pwmWriteFw);
    }

    void leftShift(int analogVar){
    
        abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(analogVar * -1, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteBk);
        motor2.write(pwmWriteFw);
        motor3.write(pwmWriteFw);
        motor4.write(pwmWriteBk);
    }

    void CW(int analogVar){
    
        abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(analogVar * -1, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteFw);
        motor2.write(pwmWriteBk);
        motor3.write(pwmWriteFw);
        motor4.write(pwmWriteBk);
    }

    void CCW(int analogVar){
    
        abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(analogVar * -1, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.write(pwmWriteBk);
        motor2.write(pwmWriteFw);
        motor3.write(pwmWriteBk);
        motor4.write(pwmWriteFw);
    }

};