#include <Arduino.h>

float normalDuration = 300; //TBD normal duration of ultrasonic pulse at floor distance
unsigned long currentMillis;
unsigned long prevMillisPulse; //last time sensors were pulsed
unsigned long prevMillisDebounce; //last time sensors were pulsed
unsigned long sensorTrigDelay = 50; //delay between sensor pulses ms
unsigned long sensorDebounce = 500; //Amount of time before sensor will say there is no longer an object ms
bool objectInPlow;
int sensorIndex = 1;     //which sensor to pulse

//pins which will trigger ultrasonic pulse
int trigPin1 = 5;
int trigPin2 = 17;
int trigPin3 = 19;
int trigPin4 = 18;

//pins which will recieve ultrasonic pulse time
int echoPin1 = 34;
int echoPin2 = 35;
int echoPin3 = 32;
int echoPin4 = 33;

bool singleSensorDetect(int trigPin, int echoPin);
bool objectDetected();


void setup(){

    Serial.begin(115200);

////////// INPUT Pin Mode Setup ////////////
    pinMode(echoPin1, INPUT);
    pinMode(echoPin2, INPUT);
    pinMode(echoPin3, INPUT);
    pinMode(echoPin4, INPUT);

/////////// OUTPUT Pin Mode Setup/////////
    pinMode(trigPin1, OUTPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(trigPin4, OUTPUT);
    

}

void loop(){
    currentMillis = millis();

    Serial.println(objectDetected()); //0 for No | 1 for Yes
}

bool singleSensorDetect(int trigPin, int echoPin){
    
    float duration; //how long between pulse and reception

    digitalWrite(trigPin, LOW);//ensuring trig pin is in LOW
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);//pulsing ultrasonic for 10 micro seconds
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);//setting pin back to low

    duration = pulseIn(echoPin, HIGH);//measuring duration of pulse wiht pulse in
    Serial.println(duration);

    return (duration < normalDuration);//returing true if the distance is closer than the floor
}

bool objectDetected(){

    if (currentMillis - sensorTrigDelay > prevMillisPulse){ //if it has been delay time since last run, run again

        //can only pulse one sensor every delay to allow for correct measurement, if there is anything detected objectInPlow = true
             if(sensorIndex == 1) {objectInPlow = singleSensorDetect(trigPin1, echoPin1);}
        else if(sensorIndex == 2) {objectInPlow = singleSensorDetect(trigPin2, echoPin2);}
        else if(sensorIndex == 3) {objectInPlow = singleSensorDetect(trigPin3, echoPin3);}
        else if(sensorIndex == 4) {objectInPlow = singleSensorDetect(trigPin4, echoPin4);}

        

        if (++sensorIndex > 4) { sensorIndex = 1;}//increment the index and set to 1 if over 8
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