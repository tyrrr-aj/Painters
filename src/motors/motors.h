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
	
This code is strongly inspired by SparkFun's RedBotMotors library
- big thank you goes to it's creator from here!
	
29.10.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/
#include <cstddef>
#ifndef MOTORS_H
#define MOTORS_H

#include <arduino.h>

#include "../../pins.h"
#include "../encoder/encoder.h"

class Motors {
	public:
		Motors();
		void drive(int speed);
		void coast();
		void stop();
		void rightMotor(int speed);
		void leftMotor(int speed);
		void addEncoder(Encoder*);
	
	private:
		void rightCoast();
		void leftCoast();
		void rightStop();
		void leftStop();
		void rightFwd(uint8_t speed);
		void leftFwd(uint8_t speed);
		void rightRev(uint8_t speed);
		void leftRev(uint8_t speed);
		Encoder* encoder;
		void setEncoderDirection(WHEEL, DIRECTION);
};

#endif