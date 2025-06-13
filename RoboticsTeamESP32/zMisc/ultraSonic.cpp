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

    return (duration < normalDuration);//returing true if the distance is closer than the floor
}


bool ultraSonic::objectDetected(){

    if (currentMillis - sensorTrigDelay > prevMillisPulse){ //if it has been delay time since last run, run again

        //can only pulse one sensor every delay to allow for correct measurement, if there is anything detected objectInPlow = true
             if(sensorIndex == 1) {objectInPlow = singleSensorDetect(trigPin1, echoPin1);}
        else if(sensorIndex == 2) {objectInPlow = singleSensorDetect(trigPin2, echoPin2);}
        else if(sensorIndex == 3) {objectInPlow = singleSensorDetect(trigPin3, echoPin3);}
        else if(sensorIndex == 4) {objectInPlow = singleSensorDetect(trigPin4, echoPin4);}

        

        if (++sensorIndex > 8) { sensorIndex = 1;}//increment the index and set to 1 if over 8
        prevMillisPulse = millis(); //set previous milliseconds to test for next time funtion is run
        
    }
    //debounce timer
    if(objectInPlow){//return true of object detected
        Serial.print("Test");
        return true;
        prevMillisDebounce = millis();
    }
    else if (currentMillis - sensorDebounce > prevMillisDebounce) return false;//if object has not been detected in sensorDebounce time return false
    
    return false;
}