#ifndef MOTORCONTROL_HPP
#define MOTORCONTROL_HPP
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>

class motorControl{
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



    void runAtSetup();
    void forward(int analogVar);
    void backward(int analogVar);
    void rightShift(int analogVar);
    void leftShift(int analogVar);
    void CW(int analogVar);
    void CCW(int analogVar);


};


#endif //motion_H