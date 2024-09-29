#ifndef ULTRASONIC_HPP
#define ULTRASONIC_HPP
#include <Arduino.h>

class ultraSonic{
    private:

    unsigned long prevMillis; //last time sensors were pulsed
    int sensorIndex; //which sensor to pulse

    bool singleSensorDetect(int trigPin, int echoPin); //pulse and test on one sensor

    public:

    unsigned long currentMillis;//current time in miliseconds
    long sensorTrigDelay; //delay between sensor pulses
    int normalDuration; //normal duration of ultrasonic pulse from floor

    //pins which will trigger ultrasonic pulse
    int trigPin1;
    int trigPin2;
    int trigPin3;
    int trigPin4;
    int trigPin5;
    int trigPin6;
    int trigPin7;
    int trigPin8;

    //pins which will recieve ultrasonic pulse time
    int echoPin1;
    int echoPin2;
    int echoPin3;
    int echoPin4;
    int echoPin5;
    int echoPin6;
    int echoPin7;
    int echoPin8;

    void runAtSetup();
    bool objectDetected();

};



#endif //ULTRASONIC_HPP