#include <Arduino.h>
#include "ultraSonic.hpp"

bool ultraSonic::singleSensorDetect(int trigPin, int echoPin){
    
    float duration; //how long between pulse and reception

    digitalWrite(trigPin, LOW);//ensuring trig pin is in LOW
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);//pulsing ultrasonic for 10 micro seconds
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);//setting pin back to low

    duration = pulseIn(echoPin, HIGH);//measuring duration of pulse wiht pulse in

    return (duration < normalDuration) ? true : false;//returing true if the distance is closer than the floor
}

void ultraSonic::runAtSetup(){

    //set trig pins to output mode
    pinMode(trigPin1,OUTPUT);
    pinMode(trigPin2,OUTPUT);
    pinMode(trigPin3,OUTPUT);
    pinMode(trigPin4,OUTPUT);
    pinMode(trigPin5,OUTPUT);
    pinMode(trigPin6,OUTPUT);
    pinMode(trigPin7,OUTPUT);
    pinMode(trigPin8,OUTPUT);

    //set echo pins to input
    pinMode(echoPin1,INPUT);
    pinMode(echoPin2,INPUT);
    pinMode(echoPin3,INPUT);
    pinMode(echoPin4,INPUT);
    pinMode(echoPin5,INPUT);
    pinMode(echoPin6,INPUT);
    pinMode(echoPin7,INPUT);
    pinMode(echoPin8,INPUT);
}

bool ultraSonic::objectDetected(){
    bool object;

    if (currentMillis - sensorTrigDelay > prevMillis){ //if it has been delay time since last run, run again

        //can only pulse one sensor every delay to allow for correct measurement, if there is anything detected return true
             if(sensorIndex == 1) {return singleSensorDetect(trigPin1, echoPin1) ? true : false;}
        else if(sensorIndex == 2) {return singleSensorDetect(trigPin2, echoPin2) ? true : false;}
        else if(sensorIndex == 3) {return singleSensorDetect(trigPin3, echoPin3) ? true : false;}
        else if(sensorIndex == 4) {return singleSensorDetect(trigPin4, echoPin4) ? true : false;}
        else if(sensorIndex == 5) {return singleSensorDetect(trigPin5, echoPin5) ? true : false;}
        else if(sensorIndex == 6) {return singleSensorDetect(trigPin6, echoPin6) ? true : false;}
        else if(sensorIndex == 7) {return singleSensorDetect(trigPin7, echoPin7) ? true : false;}
        else if(sensorIndex == 8) {return singleSensorDetect(trigPin8, echoPin8) ? true : false;}
        

        if (++sensorIndex > 8) { sensorIndex = 1;}//increment the index
        prevMillis = millis(); //set previous milliseconds to test for next time funtion is run
    }

}