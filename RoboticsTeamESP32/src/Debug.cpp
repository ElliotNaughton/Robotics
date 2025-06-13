#include <ps5Controller.h>
#include <ESP32Servo.h>
#include <HUSKYLENS.h>
#include <Wire.h>

bool isNotConnected = true;
bool ranonce = true;
bool STATE = LOW;


bool objectInPlow;
float normalDuration = 300; //TBD normal duration of ultrasonic pulse at floor distance

HUSKYLENS huskylens;

////////////// Automatic Mode Variables ////////////////////////////
int xPos = 0;
int yPos = 0;
int totalStepNumber = 0;
bool isMoving = false;
long stopTime = 50;
long moveTime = 1000;
int requestedMove;
int rotation = 1; //Starts at 1, 2 is 1 turn to the right, 3 is opposite of starting direction, 4 is 1 turn to the left, 
bool stoppedFlag = false;
bool seqStartFlag = false;
bool objectDetectedflag = false;
int robotStartArea = 0;//1 for field to right, 0 for field to left
bool readyToDropFlag = false;
bool readyForResetFlag = false;
bool objectGrabbedFlag = false;
bool firstStartFlag = true;




/////////////////////// Pin Setup /////////////////////////////////////

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

int internalLEDPin = 2;  //Internal LED on ESP32 connected to GPIO2
int sensorIndex = 1;     //which sensor to pulse
int freezeButtonPin = 27; //Freeze button inpin pin
int freezeLEDPin = 26;  //Freeze Button LED pin
int plowLiftPin = 15; //pin for relay to actuators to lift plow


int mode = 1;//0 For Stopped, 1 Manual Mode, 2 for Start Automatic Mode 

bool isFrozen = false;
bool plowState = false; //tracks state of plow, either up (true) or down (false)



///////////////////////////// Millisecond Timer Setup ////////////////////////////////
unsigned long prevMillisMove;
unsigned long prevMillisStop;
unsigned long prevMillisFreeze;
unsigned long prevMillisAuto;
unsigned long currentMillis;
unsigned long blinkMillis;
unsigned long prevMillisPulse; //last time sensors were pulsed
unsigned long prevMillisDebounce; //last time sensors were pulsed
long sensorTrigDelay = 50; //delay between sensor pulses ms
long sensorDebounce = 500; //Amount of time before sensor will say there is no longer an object ms

/////////////////////// Motor/Servo Setup ////////////////////////////
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo servo1;
int motor1pwm;
int motor2pwm;
int motor3pwm;
int motor4pwm;
int servo1pwm;

////////////////////PS5 Controller Setup////////////////////////////////
int analogVarX; //X analog stick variable
int analogVarY; //Y analog stick variable
int analogVarF;
int maxUs = 1950; //Maximum Limit of PWM (1950)
int midUs = 1500; //Mid point of PWM (1500)
int minUs = 1000; //Minumum Limit of PWM (1000)
int ps5MaxAnalogPos = 125;
int ps5MaxAnalogNeg = -125;
int ps5Deadzone = 4;
int servo1ClosedPwm = 500;
int servo1OpenPwm = 1800;

///////////////////// Function Declerations//////////////////////
void ps5ReadOrAuto(void * parameter); //task to read from ps5 controller and write to motor pwm
void fwMove();//Forward move in auto
void bwMove();//Backward move in auto
void ccwTurn();//Counter Clockwise move in auto
void cwTurn();//Clockwise move in auto
void stop();
bool singleSensorDetect(int trigPin, int echoPin);//one ultra sonic sensor pulse and read
bool objectDetected();//multiple ultra sonic sensor pulse and read


void setup()
{
  ///////////// Setting miliseconds to current time //////////////
  currentMillis = millis();
  prevMillisFreeze = millis();
  blinkMillis = millis();
  prevMillisMove = millis();
  prevMillisStop = millis();
  prevMillisDebounce = millis();
  prevMillisPulse = millis();
  prevMillisAuto = millis();


///////////
  Serial.begin(115200);
  ps5.begin("bc:c7:46:42:c6:8c");



  motor1.attach(12); //front left motor
  motor2.attach(13); //front right motor
  motor3.attach(14); //back left motor
  motor4.attach(25); //back right motor
  servo1.attach(4);  //close open plow servo

 ////////// INPUT Pin Mode Setup ////////////
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(echoPin4, INPUT);
  pinMode(freezeButtonPin, OUTPUT); //For Some reason freeze button works better in output mode

/////////// OUTPUT Pin Mode Setup/////////
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(freezeLEDPin, OUTPUT);
  pinMode(internalLEDPin,OUTPUT);
  pinMode(plowLiftPin, OUTPUT);

////////Task to Read from the PS5 Controller and write to Motors, on Core 1 due to all overhead being on Core 0///////
  xTaskCreatePinnedToCore(//task to read from ps5 controller and write to motor pwm
      ps5ReadOrAuto,    // Function that should be called
      "ps5 Read PWM Write",  // Name of the task (for debugging)
      10000,            // Stack size (bytes)
      NULL,            // Parameter to pass
      5,               // Task priority
      NULL,             // Task handle
      1               // Task pinned to core 1
  );

  Wire.begin();
 
Serial.println("Ready."); //Printing ready to the serial monitor when setup is finished
}

void loop()
{
   vTaskDelete(NULL);//deleting the loop task
}

void ps5ReadOrAuto(void * parameter){//task to read from ps5 controller and write to motor pwm
  for(;;){//start infinite loop freeRTOS can break out of loop for other tasks
    currentMillis = millis();

    
    ////////////// PS5 Connection Diagnostic LED //////////////////
    if (ps5.isConnected() && isNotConnected){
      Serial.println("Connected!");
      isNotConnected = false;
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

      //Right stick to the Left
      if((ps5.data.analog.stick.rx > ps5Deadzone)){ 
          analogVarX = abs(ps5.data.analog.stick.rx);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
      }

      //Right stick to the right
      else if((ps5.data.analog.stick.rx < -ps5Deadzone)){ 
        analogVarX = abs(ps5.data.analog.stick.rx);
        
        //Writing pwm signal for motor controllers
        motor1pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
        motor2pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
        motor3pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
        motor4pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
      }

      
        //Left stick 45 degree up and left
      else if((ps5.data.analog.stick.lx > ps5Deadzone) && (ps5.data.analog.stick.ly > ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);

          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarX - analogVarY , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor2pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor4pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
      }

      //Left stick straight up
      else if((ps5.data.analog.stick.ly > ps5Deadzone) && (ps5.data.analog.stick.lx < ps5Deadzone) && (ps5.data.analog.stick.lx > -ps5Deadzone)){
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor4pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);

      }

      //Left stick up and to the right
      else if((ps5.data.analog.stick.lx < -ps5Deadzone) && (ps5.data.analog.stick.ly > ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
        
          //Writing pwm signal for motor controllers
          motor1pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor3pwm = map(analogVarX - analogVarY , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor4pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
      }

      //Left stick straight left
      else if((ps5.data.analog.stick.lx > ps5Deadzone) && (ps5.data.analog.stick.ly < ps5Deadzone) && (ps5.data.analog.stick.ly > -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor2pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor3pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor4pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
      }

        //Left stick straight right
      else if((ps5.data.analog.stick.lx < -ps5Deadzone) && (ps5.data.analog.stick.ly < ps5Deadzone) && (ps5.data.analog.stick.ly > -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);
          
          //Writing pwm signal for motor controllers
          motor1pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor2pwm = map(-analogVarX , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor3pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(analogVarX , 0, ps5MaxAnalogPos, midUs, maxUs);
      }

      //Left stick down and to the left
      else if((ps5.data.analog.stick.lx > ps5Deadzone) && (ps5.data.analog.stick.ly < -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx + 1);
          analogVarY = abs(ps5.data.analog.stick.ly);
          //Writing pwm signal for motor controllers

          motor1pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor2pwm = map(analogVarX - analogVarY, ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor3pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor4pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);      
      }

      //Left stick straight down
      else if((ps5.data.analog.stick.ly < -ps5Deadzone) && (ps5.data.analog.stick.lx < ps5Deadzone) && (ps5.data.analog.stick.lx > -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);

          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor2pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
          motor3pwm = map(analogVarY , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(-analogVarY , ps5MaxAnalogNeg, 0, minUs, midUs);
      }

      //Left stick down and to the right
      else if((ps5.data.analog.stick.lx < -ps5Deadzone) && (ps5.data.analog.stick.ly < -ps5Deadzone)){
          analogVarX = abs(ps5.data.analog.stick.lx);
          analogVarY = abs(ps5.data.analog.stick.ly + 1);

          //Writing pwm signal for motor controllers
          motor1pwm = map(analogVarY - analogVarX , ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs);
          motor2pwm = map(-((analogVarX + analogVarY) / 2) , ps5MaxAnalogNeg, 0, minUs, midUs);   
          motor3pwm = map(((analogVarX + analogVarY) / 2) , 0, ps5MaxAnalogPos, midUs, maxUs);
          motor4pwm = map(analogVarX - analogVarY, ps5MaxAnalogNeg, ps5MaxAnalogPos, minUs, maxUs); 
      }

      //No Stick movement set motor PWM to stop
      else {
        motor1pwm = midUs;
        motor2pwm = midUs;
        motor3pwm = midUs;
        motor4pwm = midUs;
      }

      //L2 to open plow
      if (ps5.event.button_down.r1){
        servo1pwm = servo1OpenPwm;
      }

      //R2 to close plow
      else if (ps5.data.button.l1){
        servo1pwm = servo1ClosedPwm;
      }

      //R1 to raise plow
      if (ps5.event.button_down.triangle){
        digitalWrite(plowLiftPin,HIGH);
      }

      //L1 to lower plow
      else if (ps5.event.button_down.cross){
        digitalWrite(plowLiftPin,LOW);
      }
    }

    ///End of Manual Mode

    //Automatic Mode
    if (ps5.data.button.options) {
      mode = 2;
      prevMillisAuto = millis();
      Serial.println("In Auto");
      }
    if (mode == 2){
      if(currentMillis - 20000 < prevMillisAuto){
        Serial.println("flag1");
        if(firstStartFlag){

          motor1pwm = 1000;
          motor2pwm = 1950;
          motor3pwm = 1000;
          motor4pwm = 1950;
          firstStartFlag = false;
          prevMillisMove = millis();
        }

        if (currentMillis - 2000 > prevMillisMove){
          motor1pwm = 1500;
          motor2pwm = 1500;
          motor3pwm = 1500;
          motor4pwm = 1500;
        }
        
        
      }

       
      else{
      mode = 1;
      firstStartFlag = true;
      }






    }
    

  



    if(digitalRead(freezeButtonPin)) prevMillisFreeze = millis();
    isFrozen = (currentMillis - 15000 < prevMillisFreeze);

    if(isFrozen){
      motor1pwm = midUs;
      motor2pwm = midUs;
      motor3pwm = midUs;
      motor4pwm = midUs;
    

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
      STATE = LOW;
      digitalWrite(freezeLEDPin,STATE);
    }

  
    motor1.writeMicroseconds(constrain(motor1pwm,minUs,maxUs));
    motor2.writeMicroseconds(constrain(motor2pwm,minUs,maxUs));
    motor3.writeMicroseconds(constrain(motor3pwm,minUs,maxUs));
    motor4.writeMicroseconds(constrain(motor4pwm,minUs,maxUs));
    servo1.writeMicroseconds(servo1pwm);
    
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

void stop(){
  motor1pwm = midUs;
  motor2pwm = midUs;
  motor3pwm = midUs;
  motor4pwm = midUs;
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