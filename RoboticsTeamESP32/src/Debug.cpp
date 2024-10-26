#include <ps5Controller.h>
#include <ESP32Servo.h>
#include <HUSKYLENS.h>

bool isNotConnected = true;
bool ranonce = true;
bool STATE = LOW;

float normalDuration = 300; //TBD normal duration of ultrasonic pulse at floor distance
unsigned long prevMillisPulse; //last time sensors were pulsed
unsigned long prevMillisDebounce; //last time sensors were pulsed
bool objectInPlow;
long sensorTrigDelay = 50; //delay between sensor pulses ms
long sensorDebounce = 500; //Amount of time before sensor will say there is no longer an object ms
int speedPercentage = 0.5; //0.0 - 1.0

int internalLEDPin = 2;

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

int sensorIndex = 1; //which sensor to pulse

bool objectRumble;
int mode = 1;//0 For Stopped, 1 Manual Mode, 2 for Start Automatic Mode 

bool isFrozen = false;
int freezeButtonPin = 27;
int freezeLEDPin = 26;


int plowLiftPin = 15; //pin for relay to actuators to lift plow
bool plowState = false; //tracks state of plow, either up (true) or down (false)

int pwmOutput;
int analogVar3;

unsigned long currentMillis;
unsigned long prevMillisGlobal;
unsigned long prevMillisFreeze;
unsigned long blinkMillis;
unsigned long fwMoveMillis;
unsigned long bkMoveMillis;
unsigned long ccwMoveMillis;
unsigned long cwMoveMillis;

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo servo1;

int analogVarX;
int analogVarY;
int analogVarF;
int motor1pwm;
int motor2pwm;
int motor3pwm;
int motor4pwm;
int servo1pwm;
int maxUs = 1950; //Maximum Limit of PWM (1950)
int midUs = 1500; //Mid point of PWM (1500)
int minUs = 1000; //Minumum Limit of PWM (1000)
int ps5MaxAnalogPos = 125;
int ps5MaxAnalogNeg = -125;
int ps5Deadzone = 4;
int servo1ClosedPwm = 1000;
int servo1OpenPwm = 1800;

void ps5ReadOrAuto(void * parameter); //task to read from ps5 controller and write to motor pwm
void fwMove();
void bwMove();
void ccwTurn();
void cwTurn();
bool singleSensorDetect(int trigPin, int echoPin);
bool objectDetected();


void setup()
{
  currentMillis = millis();
  prevMillisGlobal = millis();
  prevMillisFreeze = millis();
  blinkMillis = millis();

  pinMode(internalLEDPin,OUTPUT);

  Serial.begin(115200);
  ps5.begin("bc:c7:46:42:c6:8c");

  pinMode(plowLiftPin, OUTPUT);

  motor1.attach(13); //front left motor
  motor2.attach(12); //front right motor
  motor3.attach(14); //back left motor
  motor4.attach(25); //back right motor
  servo1.attach(4);  //close open plow servo

 

  
  //trig pin setup
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(trigPin4, OUTPUT);
  

  //echo pin setup
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(echoPin4, INPUT);
  

  //Button Pin Setting
  pinMode(freezeLEDPin, OUTPUT);
  pinMode(freezeButtonPin, OUTPUT);


  xTaskCreatePinnedToCore(//task to read from ps5 controller and write to motor pwm
      ps5ReadOrAuto,    // Function that should be called
      "ps5 Read PWM Write",  // Name of the task (for debugging)
      10000,            // Stack size (bytes)
      NULL,            // Parameter to pass
      5,               // Task priority
      NULL,             // Task handle
      1               // Task pinned to core 1
  );
 
Serial.println("Ready.");
}

void loop()
{
   vTaskDelete(NULL);
}

void ps5ReadOrAuto(void * parameter){//task to read from ps5 controller and write to motor pwm
  for(;;){//start infinite loop freeRTOS can break out of loop for other tasks
    currentMillis = millis();

    
      
      if (ps5.isConnected() && isNotConnected){
      Serial.println("Connected!");
      isNotConnected = false;
      Serial.println(xPortGetCoreID());
      digitalWrite(internalLEDPin,HIGH);
    }

    else if (!ps5.isConnected()){
      isNotConnected = true;
      digitalWrite(internalLEDPin,LOW);
    }

    /*
      Serial.print("lx: ");
      Serial.print(ps5.data.analog.stick.lx);
      Serial.print(" ly: ");
      Serial.print(ps5.data.analog.stick.ly);
      Serial.print(" rx: ");
      Serial.print(ps5.data.analog.stick.rx);
      Serial.print(" ry: ");
      Serial.println(ps5.data.analog.stick.ry);
    */

          
    ///Start of Manual Mode
    if (mode == 1){


      if((ps5.data.analog.stick.rx < -ps5Deadzone)){ 
          analogVarX = abs(ps5.data.analog.stick.rx);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor3pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor4pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
      }

        else if((ps5.data.analog.stick.rx > ps5Deadzone)){ 
          analogVarX = abs(ps5.data.analog.stick.rx);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor2pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
      }

      
        //if is in Quadrant 1
      else if((ps5.data.analog.stick.lx > ps5Deadzone) && (ps5.data.analog.stick.ly > ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);

          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarX - analogVarY , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor2pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor4pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
      }

      //in Between 1 and 2
      else if((ps5.data.analog.stick.ly > ps5Deadzone) && (ps5.data.analog.stick.lx < ps5Deadzone) && (ps5.data.analog.stick.lx > -ps5Deadzone)){
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor4pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);

      }

      //if is in quadrant 2
      else if((ps5.data.analog.stick.lx < -ps5Deadzone) && (ps5.data.analog.stick.ly > ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
        
          //Writing pwm signal for motor controllers
          motor1pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor3pwm = map(analogVarX - analogVarY , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor4pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
      }

      //in Between 1 and 3 
      else if((ps5.data.analog.stick.lx > ps5Deadzone) && (ps5.data.analog.stick.ly < ps5Deadzone) && (ps5.data.analog.stick.ly > -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor2pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor4pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
      }

        //in Between 2 and 4 
      else if((ps5.data.analog.stick.lx < -ps5Deadzone) && (ps5.data.analog.stick.ly < ps5Deadzone) && (ps5.data.analog.stick.ly > -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor3pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
      }

      //if is in Quadrant 3 
      else if((ps5.data.analog.stick.lx > ps5Deadzone) && (ps5.data.analog.stick.ly < -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly);
          //Writing pwm signal for motor controllers

          motor1pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor2pwm = map(analogVarX - analogVarY, ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor3pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor4pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);      
      }

      //in Between 3 and 4
      else if((ps5.data.analog.stick.ly < -ps5Deadzone) && (ps5.data.analog.stick.lx < ps5Deadzone) && (ps5.data.analog.stick.lx > -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);

          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor2pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor3pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
      }

      //if is in Quadrant 4
      else if((ps5.data.analog.stick.lx < -ps5Deadzone) && (ps5.data.analog.stick.ly < -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);

          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor2pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);   
          motor3pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(analogVarX - analogVarY, ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs); 
      }

        else {
          motor1pwm = midUs;
          motor2pwm = midUs;
          motor3pwm = midUs;
          motor4pwm = midUs;
      }

      if (ps5.event.button_down.r1){
        servo1pwm = servo1OpenPwm;
      }

      else if (ps5.event.button_down.r2){
        servo1pwm = servo1ClosedPwm;
      }
    }

    ///End of Manual Mode

    //Automatic Mode
    else if (mode == 2){





    }
    

    if (ranonce){ 
    Serial.println(xPortGetCoreID());
    ranonce = false;}



    if(digitalRead(freezeButtonPin)) prevMillisFreeze = millis(); 
    isFrozen = (currentMillis - 15000 < prevMillisFreeze);

    if(isFrozen){
    motor1.writeMicroseconds(midUs);
    motor2.writeMicroseconds(midUs);
    motor3.writeMicroseconds(midUs);
    motor4.writeMicroseconds(midUs);

    Serial.print("FROZEN for:");
    Serial.print(currentMillis - prevMillisFreeze);
    Serial.println(" Milliseconds");

    if(currentMillis - 250 > blinkMillis) {
      if (STATE == LOW) STATE = HIGH;
      else STATE = LOW;
      digitalWrite(freezeLEDPin,STATE);
      blinkMillis = millis();
      }
    }

    else{
    motor1.writeMicroseconds(speedPercentage / 100 * constrain(motor1pwm,minUs,maxUs));
    motor2.writeMicroseconds(speedPercentage / 100 * constrain(motor2pwm,minUs,maxUs));
    motor3.writeMicroseconds(speedPercentage / 100 * constrain(motor3pwm,minUs,maxUs));
    motor4.writeMicroseconds(speedPercentage / 100 * constrain(motor4pwm,minUs,maxUs));
    servo1.writeMicroseconds(servo1pwm);

    Serial.print("motor1:");
    Serial.print(constrain(motor1pwm,minUs,maxUs));
    Serial.print(" motor2:");
    Serial.print(constrain(motor2pwm,minUs,maxUs));
    Serial.print("motor3:");
    Serial.print(constrain(motor3pwm,minUs,maxUs));
    Serial.print(" motor4:");
    Serial.println(constrain(motor4pwm,minUs,maxUs));
    digitalWrite(freezeLEDPin,LOW);
    }

  }
}


void fwMove(){
  motor1pwm = maxUs;
  motor2pwm = minUs;
  motor3pwm = maxUs;
  motor4pwm = minUs;
}

void bwMove(){
  motor1pwm = minUs;
  motor2pwm = maxUs;
  motor3pwm = minUs;
  motor4pwm = maxUs;
}

void ccwTurn(){
  motor1pwm = minUs;
  motor2pwm = minUs;
  motor3pwm = minUs;
  motor4pwm = minUs;
}

void cwTurn(){
  motor1pwm = maxUs;
  motor2pwm = maxUs;
  motor3pwm = maxUs;
  motor4pwm = maxUs;
}

bool singleSensorDetect(int trigPin, int echoPin){
    
    float duration; //how long between pulse and reception

    digitalWrite(trigPin, LOW);//ensuring trig pin is in LOW
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);//pulsing ultrasonic for 10 micro seconds
    delayMicroseconds(10);

    digitalWrite(trigPin, LOW);//setting pin back to low

    duration = pulseIn(echoPin, HIGH);//measuring duration of pulse wiht pulse in

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