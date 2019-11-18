/*********************************************************************
This library allows use of encoders for keeping track of mobile robot's
current position (measured in "ticks" - refering to cogwheels' ticks
that can be counted for robots based on the Magician Chasiss).

Notice the "pins.h" config header file in dependecies - this library
won't work unless you set pin bindings' values there appropriately!

This file usage is intended to be beer-driven: 
if you think you've found a bug, don't contact
us, instead drink a craft beer. If the bug is
still there - repeat. Cheers!
	
This code has been inspired by SparkFun's RedBotEncoder library
- big "thank you" goes to it's creator from here!
	
18.11.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/

#include "encoder.h"

Encoder* local_encoder;

Encoder::Encoder() {
	local_encoder = this;
	this->editingLeftTicks = portMUX_INITIALIZER_UNLOCKED;
	this->editingRightTicks = portMUX_INITIALIZER_UNLOCKED;
	clear();
	initInterrupts();
}

void Encoder::setDirection(WHEEL wheel, DIRECTION direction) {
	switch (wheel) {
		case LEFT:
			leftDirection = direction;
			break;
		case RIGHT:
			rightDirection = direction;
			break;
	}
}

int Encoder::getTicks(WHEEL wheel) {
	switch (wheel) {
		case LEFT:
			return leftTicks;
		case RIGHT:
			return rightTicks;
	}
}

void Encoder::clear() {
	portENTER_CRITICAL(&(this->editingLeftTicks));
	portENTER_CRITICAL(&(this->editingRightTicks));
	this->leftTicks = 0;
	this->rightTicks = 0;
	portEXIT_CRITICAL(&(this->editingRightTicks));
	portEXIT_CRITICAL(&(this->editingLeftTicks));
	this->leftDirection = FWD;
	this->rightDirection = FWD;
}

void Encoder::initInterrupts() {
	pinMode(LEFT_ENC, INPUT_PULLUP);
	pinMode(RIGHT_ENC, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(LEFT_ENC), leftIncrement, FALLING);
	attachInterrupt(digitalPinToInterrupt(RIGHT_ENC), rightIncrement, FALLING);
}

/*********************************************************************
Methods below are only meant to be used as interrupts' callbacks
- they should never be invoked in any other context.
*********************************************************************/

void Encoder::rightIncrement() {
	portENTER_CRITICAL(&(local_encoder->editingRightTicks));
	if (local_encoder->rightDirection == FWD)
		local_encoder->rightTicks++;
	else
		local_encoder->rightTicks--;
	portEXIT_CRITICAL(&(local_encoder->editingRightTicks));
}

void Encoder::leftIncrement() {
	portENTER_CRITICAL(&(local_encoder->editingLeftTicks));
	if (local_encoder->leftDirection == FWD)
		local_encoder->leftTicks++;
	else
		local_encoder->leftTicks--;
	portEXIT_CRITICAL(&(local_encoder->editingLeftTicks));
}