#include <Arduino.h>
#include <ESP32Servo.h>


Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo servo1;
int motor1pwm;
int motor2pwm;
int motor3pwm;
int motor4pwm;
//int servo1pwm;
unsigned long currentMillis;
unsigned long prevMillis;
int maxUs = 1950; //Maximum Limit of PWM (1950)
int midUs = 1500; //Mid point of PWM (1500)
int minUs = 1000; //Minumum Limit of PWM (1000)

void setup(){

    motor1.attach(13); //front left motor
    motor2.attach(12); //front right motor
    motor3.attach(14); //back left motor
    motor4.attach(25); //back right motor
    servo1.attach(4);  //close open plow servo

    currentMillis = millis();
    prevMillis = millis();

    motor1pwm = maxUs;
    motor2pwm = minUs;
    motor3pwm = maxUs;
    motor4pwm = minUs;

}

void loop(){
    currentMillis = millis();

    if (currentMillis - 50 > prevMillis){
        
        motor1pwm = motor1pwm - 10;
        motor2pwm = motor2pwm + 10;
        motor3pwm = motor3pwm - 10;
        motor4pwm = motor4pwm + 10;
        
        if (motor1pwm < minUs || motor2pwm > maxUs){
            motor1pwm = maxUs;
            motor2pwm = minUs;
            motor3pwm = maxUs;
            motor4pwm = minUs;
        }

    }



    Serial.print("motor1:");
    Serial.print(constrain(motor1pwm,minUs,maxUs));
    Serial.print(" motor2:");
    Serial.print(constrain(motor2pwm,minUs,maxUs));
    Serial.print("motor3:");
    Serial.print(constrain(motor3pwm,minUs,maxUs));
    Serial.print(" motor4:");
    Serial.println(constrain(motor4pwm,minUs,maxUs));

    motor1.writeMicroseconds(constrain(motor1pwm,minUs,maxUs));
    motor2.writeMicroseconds(constrain(motor2pwm,minUs,maxUs));
    motor3.writeMicroseconds(constrain(motor3pwm,minUs,maxUs));
    motor4.writeMicroseconds(constrain(motor4pwm,minUs,maxUs));
    //servo1.writeMicroseconds(servo1pwm);



}