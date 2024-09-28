#include <Arduino.h>
#include <ESP32Servo.h>
#include <Ps3Controller.h>

class robot{
    private:

    Servo motor1; //front left motor
    Servo motor2; //front right motor
    Servo motor3; //back left motor
    Servo motor4; //back right motor
    Servo servo1; //close open plow motor
    Servo motor5; //motor for hanging right
    Servo motor6; //motor for hanging left
    int analogVar;
    int pwmWriteFw;  //PWM varialble for forward
    int pwmWriteBk;  //PWM variable for backward

    

    public: 
        
    int minUs;//minumum PWM
    int midUs;//mid range PWM
    int maxUs;//maximum PWM

    int motor1Pin;
    int motor2Pin;
    int motor3Pin;
    int motor4Pin;
    int servo1Pin;
    int motor5Pin;
    int motor6Pin;



    void runAtSetup();
    void forward(int analogVar);
    void backward(int analogVar);
    void rightShift(int analogVar);
    void leftShift(int analogVar);
    void CW(int analogVar);


};