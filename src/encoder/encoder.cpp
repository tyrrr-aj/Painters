/*********************************************************************
This library allows use of encoders for keeping track of mobile robot's
current position (measured in "ticks" - refering to cogwheels' ticks
that can be counted for robots based on the Magician Chasiss).

Library needs to be supported by motors library - user is responsible
for providing proper and up-to-date direction on each wheel.

Notice the "pins.h" config header file in dependecies - this library
won't work unless you set pin bindings' values there appropriately!

This file usage is intended to be beer-driven: 
if you think you've found a bug, don't contact
us, drink a craft beer instead. If bug is
still there - repeat. Cheers!
	
This code has been inspired by SparkFun's RedBotEncoder library
- big "thank you" goes to it's creator from here!
	
18.11.2019 - Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/

#include "encoder.h"

Encoder* local_encoder;

Encoder::Encoder() {
	local_encoder = this;
	editingLeftTicks = xSemaphoreCreateBinary();
	editingRightTicks = xSemaphoreCreateBinary();
	xSemaphoreGive(editingLeftTicks);
	xSemaphoreGive(editingRightTicks);
	clear();
	leftDirection = FWD;
	rightDirection = FWD;
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
	xSemaphoreTake(editingLeftTicks, (TickType_t) 30);
	xSemaphoreTake(editingRightTicks, (TickType_t) 30);
	leftTicks = 0;
	rightTicks = 0;
	xSemaphoreGive(editingLeftTicks);
	xSemaphoreGive(editingRightTicks);
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
	xSemaphoreTakeFromISR(local_encoder->editingRightTicks, NULL);
	if (local_encoder->rightDirection == FWD)
		local_encoder->rightTicks++;
	else
		local_encoder->rightTicks--;
	xSemaphoreGiveFromISR(local_encoder->editingRightTicks, NULL);
}

void Encoder::leftIncrement() {
	xSemaphoreTakeFromISR(local_encoder->editingLeftTicks, NULL);
	if (local_encoder->leftDirection == FWD)
		local_encoder->leftTicks++;
	else
		local_encoder->leftTicks--;
	xSemaphoreGiveFromISR(local_encoder->editingLeftTicks, NULL);
}