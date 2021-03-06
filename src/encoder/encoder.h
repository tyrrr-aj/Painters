/*********************************************************************
This library allows use of encoders for keeping track of mobile robot's
current position (measured in "ticks" - refering to cogwheels' ticks
that can be counted for robots based on the Magician Chasiss).

Library needs to be supported by motors library - user is responsible
for providing proper and up-to-date direction on each wheel.

Notice the "pins.h" config header file in dependecies - this library
won't work unless you set pin bindings' values there appropriately!s
	
This code has been inspired by SparkFun's RedBotEncoder library
- big "thank you" goes to it's creator from here!
	
18.11.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/

#ifndef ENCODER_H
#define ENCODER_H

#include <arduino.h>

#include "../../pins.h"

#define ENCODER_MIN_DELAY 50

enum WHEEL {LEFT, RIGHT};

enum DIRECTION {FWD, REV};

class Encoder {
	public:
		Encoder();
		void setDirection(WHEEL, DIRECTION);
		int getTicks(WHEEL);
		void clear();
		
	private:
		DIRECTION leftDirection;
		DIRECTION rightDirection;
		volatile int leftTicks;
		volatile int rightTicks;
		void initInterrupts();
		SemaphoreHandle_t editingLeftTicks;
		SemaphoreHandle_t editingRightTicks;
		static void rightIncrement();
		static void leftIncrement();
		TickType_t rightLastTickTime;
		TickType_t leftLastTickTime;
};

#endif