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

    if (currentMillis - sensorTrigDelay > prevMillisPulse){ //if it has been delay time since last run, run again

        //can only pulse one sensor every delay to allow for correct measurement, if there is anything detected objectInPlow = true
             if(sensorIndex == 1) {objectInPlow = singleSensorDetect(trigPin1, echoPin1);}
        else if(sensorIndex == 2) {objectInPlow = singleSensorDetect(trigPin2, echoPin2);}
        else if(sensorIndex == 3) {objectInPlow = singleSensorDetect(trigPin3, echoPin3);}
        else if(sensorIndex == 4) {objectInPlow = singleSensorDetect(trigPin4, echoPin4);}
        else if(sensorIndex == 5) {objectInPlow = singleSensorDetect(trigPin5, echoPin5);}
        else if(sensorIndex == 6) {objectInPlow = singleSensorDetect(trigPin6, echoPin6);}
        else if(sensorIndex == 7) {objectInPlow = singleSensorDetect(trigPin7, echoPin7);}
        else if(sensorIndex == 8) {objectInPlow = singleSensorDetect(trigPin8, echoPin8);}
        

        if (++sensorIndex > 8) { sensorIndex = 1;}//increment the index and set to 1 if over 8
        prevMillisPulse = millis(); //set previous milliseconds to test for next time funtion is run
        
    }
    //debounce timer
    if(objectInPlow){//return true of object detected
        return true;
        prevMillisDebounce = millis();
    }
    else if (currentMillis - sensorDebounce > prevMillisDebounce){return false;}//if object has not been detected in sensorDebounce time return false
    
    return false;
}