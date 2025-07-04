#ifndef ULTRASONIC_HPP
#define ULTRASONIC_HPP
#include <Arduino.h>

class ultraSonic{
    private:

    unsigned long prevMillisPulse; //last time sensors were pulsed
    unsigned long prevMillisDebounce; //last time sensors were pulsed
    
    int sensorIndex; //which sensor to pulse

    bool singleSensorDetect(int trigPin, int echoPin); //pulse and test on one sensor

    public:

    bool objectInPlow;
    unsigned long currentMillis;//current time in miliseconds
    long sensorTrigDelay; //delay between sensor pulses
    long sensorDebounce; //Amount of time before sensor will say there is no longer an object
    float normalDuration; //normal duration of ultrasonic pulse from floor

    //pins which will trigger ultrasonic pulse
    int trigPin1;
    int trigPin2;
    int trigPin3;
    int trigPin4;
 

    //pins which will recieve ultrasonic pulse time
    int echoPin1;
    int echoPin2;
    int echoPin3;
    int echoPin4;


    bool objectDetected();

};



#endif //ULTRASONIC_HPP