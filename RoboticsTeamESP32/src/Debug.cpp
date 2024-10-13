#include <Ps3Controller.h>
#include <ESP32Servo.h>

bool isNotConnected = true;
Servo Test1;

int ps3StickDeadzone = 10; //what number to start tracking value changes for analog stick reading

int plowLiftPin = 23; //pin for relay to actuators to lift plow
bool plowState = false; //tracks state of plow, either up (true) or down (false)

int pwmOutput;
int analogVar3;

Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo servo1;

int ps3MaxAnalogPos = 130;
int ps3MaxAnalogNeg = -130;
int minUs = 1000;
int midUs = 1500;
int maxUs = 1950;


void forward(int analogVar){

        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteFw);
        motor2.writeMicroseconds(pwmWriteFw);
        motor3.writeMicroseconds(pwmWriteFw);
        motor4.writeMicroseconds(pwmWriteFw);

        Serial.print(pwmWriteFw);
        Serial.println("Forward");

    };

void backward(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteBk);
        motor2.writeMicroseconds(pwmWriteBk);
        motor3.writeMicroseconds(pwmWriteBk);
        motor4.writeMicroseconds(pwmWriteBk);

    };

void rightShift(int analogVar){
        
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteFw);
        motor2.writeMicroseconds(pwmWriteBk);
        motor3.writeMicroseconds(pwmWriteBk);
        motor4.writeMicroseconds(pwmWriteFw);
    }

void leftShift(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteBk);
        motor2.writeMicroseconds(pwmWriteFw);
        motor3.writeMicroseconds(pwmWriteFw);
        motor4.writeMicroseconds(pwmWriteBk);
    }

void CW(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteFw);
        motor2.writeMicroseconds(pwmWriteBk);
        motor3.writeMicroseconds(pwmWriteFw);
        motor4.writeMicroseconds(pwmWriteBk);
    }

void CCW(int analogVar){
    
        analogVar = abs(analogVar);   //Ensuring the number is positive
        int pwmWriteFw;  //PWM varialble for forward
        int pwmWriteBk;  //PWM variable for backward

        pwmWriteFw = map(analogVar, 0, ps3MaxAnalogPos, midUs, maxUs); //Mapping analog value from controller to pwm for forward 
        pwmWriteBk = map(-analogVar, ps3MaxAnalogNeg, 0, minUs, midUs);  //Mapping analog value from controller to pwm for backward

        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(pwmWriteBk);
        motor2.writeMicroseconds(pwmWriteFw);
        motor3.writeMicroseconds(pwmWriteBk);
        motor4.writeMicroseconds(pwmWriteFw);
    }

void Stop(){
  
        //Writing pwm signal for motor controllers
        motor1.writeMicroseconds(midUs);
        motor2.writeMicroseconds(midUs);
        motor3.writeMicroseconds(midUs);
        motor4.writeMicroseconds(midUs);
    }


void setup()
{

  Serial.begin(115200);
  Ps3.begin("2c:81:58:49:e4:01");
  Serial.println("Ready.");

  pinMode(plowLiftPin, OUTPUT);

  motor1.attach(13); //front left motor
  motor2.attach(12); //front right motor
  motor3.attach(14); //back left motor
  motor4.attach(27); //back right motor
  servo1.attach(15);  //close open plow servo


  
}

void loop()
{
  if (Ps3.isConnected() && isNotConnected){
    Serial.println("Connected!");
    isNotConnected = false;
  }


//Horizontal Movement of the Left Stick
  if(Ps3.event.analog_changed.stick.lx){

      if(Ps3.data.analog.stick.lx > ps3StickDeadzone){rightShift(Ps3.data.analog.stick.lx);}

      if(Ps3.data.analog.stick.lx < -ps3StickDeadzone){leftShift(Ps3.data.analog.stick.lx);}
  }

  //Vertical movement of the Left sick
  if(Ps3.event.analog_changed.stick.ly){
    
    if(Ps3.data.analog.stick.ly < -ps3StickDeadzone){forward(Ps3.data.analog.stick.ly);}

    if(Ps3.data.analog.stick.ly > ps3StickDeadzone){backward(Ps3.data.analog.stick.ly);}
  }  

  //Horizontal movement of the Right Stick
  if(Ps3.event.analog_changed.stick.rx){

    if(Ps3.data.analog.stick.rx > ps3StickDeadzone){CW(Ps3.data.analog.stick.rx);}

    if(Ps3.data.analog.stick.rx < -ps3StickDeadzone){CCW(Ps3.data.analog.stick.rx);}
  }

  //Vertical movement of the Right Stick
  if(Ps3.event.analog_changed.stick.ry){


    }  



  //right bumper
  if(Ps3.event.button_down.r1){ 
    digitalWrite(plowLiftPin,HIGH);
  }

    //left bumper
  if(Ps3.event.button_down.l1){ 
    digitalWrite(plowLiftPin,LOW);
  }


  if(    Ps3.data.analog.stick.lx < ps3StickDeadzone && Ps3.data.analog.stick.lx > -ps3StickDeadzone 
      && Ps3.data.analog.stick.ly < ps3StickDeadzone && Ps3.data.analog.stick.ly > -ps3StickDeadzone  
      && Ps3.data.analog.stick.rx < ps3StickDeadzone && Ps3.data.analog.stick.rx > -ps3StickDeadzone 
      && Ps3.data.analog.stick.ry < ps3StickDeadzone && Ps3.data.analog.stick.ry > -ps3StickDeadzone){
        Stop();
      }

}