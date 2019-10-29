/*********************************************************************
Library for controlling motors of robot based on TB6612 FNG hardware 
motor driver. It takes care of setting appropriate pin states and
provides a high-level API for commanding motors.

Notice the "pins.h" config header file in dependecies - this library
won't work unless you set pin bindings' values there appropriately.

This file usage is intended to be beer-driven: 
if you think you've found a bug, don't contact
us, instead drink a craft beer. If the bug is
still there - repeat. Cheers!
	
This code is strongly inspired by SparkFun's RedBotMotors library
- big thank you goes to it's creator from here!
	
29.10.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/
#include "pins.h"
#include "motors.h"

/* configuration of ledc channels (ESP32 internal PWM generator)
If you're using other libraries that depends on ledc, make sure
the channels do not interferre and change them if needed. */

#define PWM_FREQ 10000
#define PWM_RES 8

#define RIGHT_MOTOR_PWM_CHANNEL 0
#define LEFT_MOTOR_PWM_CHANNEL 1

//extern const int RIGHT_MOTOR_IN1;
//extern const int RIGHT_MOTOR_IN2;
//
//extern const int LEFT_MOTOR_IN1;
//extern const int LEFT_MOTOR_IN2;
//
//extern const int RIGHT_MOTOR_PWM;
//extern const int LEFT_MOTOR_PWM;

Motors::Motors() {}

void Motors::init() {
	pinMode(RIGHT_MOTOR_IN1, OUTPUT);
	pinMode(RIGHT_MOTOR_IN2, OUTPUT);
	pinMode(LEFT_MOTOR_IN1, OUTPUT);
	pinMode(LEFT_MOTOR_IN2, OUTPUT);
	
	ledcSetup(RIGHT_MOTOR_PWM_CHANNEL, PWM_FREQ, PWM_RES);
	ledcSetup(LEFT_MOTOR_PWM_CHANNEL, PWM_FREQ, PWM_RES);
	
	ledcAttachPin(RIGHT_MOTOR_PWM, RIGHT_MOTOR_PWM_CHANNEL);
	ledcAttachPin(LEFT_MOTOR_PWM, LEFT_MOTOR_PWM_CHANNEL);
}

void Motors::drive(int speed) {
	rightMotor(speed);
	leftMotor(speed);
}

void Motors::coast() {
	rightCoast();
	leftCoast();
}

void Motors::stop() {
	rightStop();
	leftStop();
}

void Motors::rightMotor(int speed) {
	if (speed > 0) {
		rightFwd((byte) speed);
	}
	else {
		rightRev((byte) abs(speed));
	}
}

void Motors::leftMotor(int speed) {
	if (speed > 0) {
		leftFwd((byte) speed);
	}
	else {
		leftRev((byte) abs(speed));
	}
}

/****************************************************
All methods below are private and strongly connected
to TB6612 FNG working details
****************************************************/

void Motors::rightFwd(byte speed) {
	digitalWrite(RIGHT_MOTOR_IN1, HIGH);
	digitalWrite(RIGHT_MOTOR_IN2, LOW);
	ledcWrite(RIGHT_MOTOR_PWM_CHANNEL, speed);
}

void Motors::leftFwd(byte speed) {
	digitalWrite(LEFT_MOTOR_IN1, LOW);
	digitalWrite(LEFT_MOTOR_IN2, HIGH);
	ledcWrite(LEFT_MOTOR_PWM_CHANNEL, speed);
}

void Motors::rightRev(byte speed) {
	digitalWrite(RIGHT_MOTOR_IN1, LOW);
	digitalWrite(RIGHT_MOTOR_IN2, HIGH);
	ledcWrite(RIGHT_MOTOR_PWM_CHANNEL, speed);
}

void Motors::leftRev(byte speed) {
	digitalWrite(LEFT_MOTOR_IN1, HIGH);
	digitalWrite(LEFT_MOTOR_IN2, LOW);
	ledcWrite(LEFT_MOTOR_PWM_CHANNEL, speed);
}

void Motors::rightCoast() {
	digitalWrite(RIGHT_MOTOR_IN1, LOW);
	digitalWrite(RIGHT_MOTOR_IN2, LOW);
}

void Motors::leftCoast() {
	digitalWrite(LEFT_MOTOR_IN1, LOW);
	digitalWrite(LEFT_MOTOR_IN2, LOW);
}

void Motors::rightStop() {
	digitalWrite(RIGHT_MOTOR_IN1, HIGH);
	digitalWrite(RIGHT_MOTOR_IN2, HIGH);
}

void Motors::leftStop() {
	digitalWrite(LEFT_MOTOR_IN1, HIGH);
	digitalWrite(LEFT_MOTOR_IN2, HIGH);
}