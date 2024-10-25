#include <Ps3Controller.h>
#include <ESP32Servo.h>
#include <HUSKYLENS.h>

bool isNotConnected = true;

float normalDuration = 300; //TBD normal duration of ultrasonic pulse at floor distance
unsigned long prevMillisPulse; //last time sensors were pulsed
unsigned long prevMillisDebounce; //last time sensors were pulsed
bool objectInPlow;
long sensorTrigDelay = 50; //delay between sensor pulses ms
long sensorDebounce = 500; //Amount of time before sensor will say there is no longer an object ms

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
int mode = 0;//0 For Stopped, 1 Manual Mode, 2 for Start Automatic Mode 

bool isFrozen;
int freezeButtonPin = 25;
int freezeLEDPin = 26;


int plowLiftPin = 23; //pin for relay to actuators to lift plow
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
int maxUs = 1950;
int midUs = 1500;
int minUs = 1000;
int ps3MaxAnalogPos = 125;
int ps3MaxAnalogNeg = -125;
int ps3Deadzone = 4;
int servo1ClosedPwm = 1000;
int servo1OpenPwm = 1800;

void ps3ReadOrAuto(void * parameter); //task to read from ps3 controller and write to motor pwm
void writeToMotors(void * parameter); //task write PWM to motors
void continuosDetection(void * parameter); //task to read and pulse ultrasonic
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

  Serial.begin(115200);
  Ps3.begin("2c:81:58:49:e4:01");
  Serial.println("Ready.");

  pinMode(plowLiftPin, OUTPUT);

  motor1.attach(13); //front left motor
  motor2.attach(12); //front right motor
  motor3.attach(14); //back left motor
  motor4.attach(2); //back right motor
  servo1.attach(4);  //close open plow servo

 


  //trig pin setup
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(trigPin4, OUTPUT);

  //echo pin setup
  pinMode(echoPin1, OUTPUT);
  pinMode(echoPin2, OUTPUT);
  pinMode(echoPin3, OUTPUT);
  pinMode(echoPin4, OUTPUT);

  //Button Pin Setting
  pinMode(freezeLEDPin, OUTPUT);
  pinMode(freezeButtonPin, OUTPUT);




  
}

void loop()
{
}
