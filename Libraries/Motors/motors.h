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
#ifndef MOTORS_H
#define MOTOR_H

#include <arduino.h>
#include "pins.h"

class Motors {
	public:
		Motors();
		void drive(int speed);
		void coast();
		void stop();
		void rightMotor(int speed);
		void leftMotor(int speed);
	
	private:
		void rightFwd(byte speed);
		void leftFwd(byte speed);
		void rightRev(byte speed);
		void leftRev(byte speed);
		void rightCoast();
		void leftCoast();
		void rightBrake();
		void leftBrake();
}

#endif