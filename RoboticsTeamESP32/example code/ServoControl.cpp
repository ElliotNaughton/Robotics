/*
 * ESP32 Servo Example Using Arduino ESP32 Servo Library
 * John K. Bennett
 * March, 2017
 * 
 * This sketch uses the Arduino ESP32 Servo Library to sweep 4 servos in sequence.
 * 
 * Different servos require different pulse widths to vary servo angle, but the range is 
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 * 
 * Circuit:
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 * 
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pins
 * 22, 19, 23, & 18).
 * 
 * In this example, we assume four Tower Pro SG90 small servos.
 * The published min and max for this servo are 500 and 2400, respectively.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
 * Experimentally, 550 and 2350 are pretty close to 0 and 180.
 */

#include <Arduino.h>
#include <ESP32Servo.h>

// create four servo objects 
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;
Servo servo1;
// Published values for SG90 servos; adjust if needed
int minUs = 1050;
int maxUs = 1950;

// These are all GPIO pins on the ESP32
// Recommended pins include 2,4,12-19,21-23,25-27,32-33
// for the ESP32-S2 the GPIO pins are 1-21,26,33-42
// for the ESP32-S3 the GPIO pins are 1-21,35-45,47-48
// for the ESP32-C3 the GPIO pins are 1-10,18-21

int motor1Pin = 11;
int motor2Pin = 12;
int motor3Pin = 13;
int motor4Pin = 14;
int servo1Pin = 4;

ESP32PWM pwm;
void setup() {
	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	Serial.begin(115200);
	motor1.setPeriodHertz(50);      // Standard 50hz servo
	motor2.setPeriodHertz(50);      // Standard 50hz servo
	motor3.setPeriodHertz(50);      // Standard 50hz servo
	motor4.setPeriodHertz(50);      // Standard 50hz servo
	//servo1.setPeriodHertz(50);      // Standard 50hz servo


}

void loop() {
	motor1.attach(motor1Pin, minUs, maxUs);
	motor2.attach(motor2Pin, minUs, maxUs);
	motor3.attach(motor3Pin, minUs, maxUs);
	motor4.attach(motor4Pin, minUs, maxUs);

	servo1.attach(servo1Pin, minUs, maxUs);

	pwm.attachPin(27, 10000);//10khz




	motor1.detach();
	motor2.detach();;
	motor3.detach();
	motor4.detach();
	pwm.detachPin(27);

}
