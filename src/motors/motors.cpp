/*********************************************************************
Library for controlling motors of robot based on TB6612 FNG hardware 
motor driver. It takes care of setting appropriate pin states and
provides a high-level API for commanding motors.

Library can be used together with the "encoder.h" library 
- if an encoder object is added by Motors::addEncoder method, from that
moment on this encoder's directions will be automatically updated
(by using setDirection(WHEEL, DIRECTION) method - be sure your 
encoder controller supports it or adjust Motors::setEncoderDirection
method respectively).

Notice the "pins.h" config header file in dependecies - this library
won't work unless you set pin bindings' values there appropriately!

This file usage is intended to be beer-driven: 
if you think you've found a bug, don't contact
us, drink a craft beer instead. If bug is
still there - repeat. Cheers!
	
This code is strongly inspired by SparkFun's RedBotMotors library
- big "thank you" goes to it's creator from here!
	
29.10.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/
#include "motors.h"

/* configuration of ledc channels (ESP32 internal PWM generator)
If you're using other libraries that depend on ledc, make sure
the channels do not interferre and change them if needed. */

#define PWM_FREQ 10000
#define PWM_RES 8

#define RIGHT_MOTOR_PWM_CHANNEL 0
#define LEFT_MOTOR_PWM_CHANNEL 1

Motors::Motors() {
	pinMode(RIGHT_MOTOR_IN1, OUTPUT);
	pinMode(RIGHT_MOTOR_IN2, OUTPUT);
	pinMode(LEFT_MOTOR_IN1, OUTPUT);
	pinMode(LEFT_MOTOR_IN2, OUTPUT);
	
	ledcSetup(RIGHT_MOTOR_PWM_CHANNEL, PWM_FREQ, PWM_RES);
	ledcSetup(LEFT_MOTOR_PWM_CHANNEL, PWM_FREQ, PWM_RES);
	
	ledcAttachPin(RIGHT_MOTOR_PWM, RIGHT_MOTOR_PWM_CHANNEL);
	ledcAttachPin(LEFT_MOTOR_PWM, LEFT_MOTOR_PWM_CHANNEL);
	
	this->encoder = NULL;
}

void Motors::addEncoder(Encoder* encoder) {
	this->encoder = encoder;
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
		setEncoderDirection(RIGHT, FWD);
	}
	else {
		rightRev((byte) abs(speed));
		setEncoderDirection(RIGHT, REV);
	}
}

void Motors::leftMotor(int speed) {
	if (speed > 0) {
		leftFwd((byte) speed);
		setEncoderDirection(LEFT, FWD);
	}
	else {
		leftRev((byte) abs(speed));
		setEncoderDirection(LEFT, REV);
	}
}

/****************************************************
All methods below are private and strongly dependant
on TB6612 FNG working details
****************************************************/

void Motors::rightFwd(int speed) {
	digitalWrite(RIGHT_MOTOR_IN1, HIGH);
	digitalWrite(RIGHT_MOTOR_IN2, LOW);
	ledcWrite(RIGHT_MOTOR_PWM_CHANNEL, speed);
}

void Motors::leftFwd(int speed) {
	digitalWrite(LEFT_MOTOR_IN1, LOW);
	digitalWrite(LEFT_MOTOR_IN2, HIGH);
	ledcWrite(LEFT_MOTOR_PWM_CHANNEL, speed);
}

void Motors::rightRev(int speed) {
	digitalWrite(RIGHT_MOTOR_IN1, LOW);
	digitalWrite(RIGHT_MOTOR_IN2, HIGH);
	ledcWrite(RIGHT_MOTOR_PWM_CHANNEL, speed);
}

void Motors::leftRev(int speed) {
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

/****************************************************
Private method for handling the Encoder object
****************************************************/

void Motors::setEncoderDirection(WHEEL wheel, DIRECTION direction) {
	if (this->encoder != NULL)
		this->encoder->setDirection(wheel, direction);
}