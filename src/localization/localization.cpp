/*********************************************************************
This library allows use of encoders for keeping track of mobile robot's
relative position in Cartesian system. Distances are measured
in centimeters, while rotation angle is given in degrees.

Library makes use of Encoder library - be sure to have one configured
and active while using Localization module.

This file usage is intended to be beer-driven: 
if you think you've found a bug, don't contact
us, drink a craft beer instead. If bug is
still there - repeat. Cheers!
	
18.11.2019- Adam Szreter, AGH University of Science and Technology
Code developed in Arduino 1.8.9, on ESP32 DevkitC v4
*********************************************************************/
#include "localization.h"

#include <math.h>

Localization::Localization(Encoder* encoder) {
	this->encoder = encoder;
	currentPosition.X = currentPosition.Y = currentPosition.Rotation = 0;
}

Position Localization::getCurrentPosition() {
	updatePosition();
	return this->currentPosition;
}

/*********************************************************************
All methods below are private.
*********************************************************************/

void Localization::updatePosition() {
	int rightTicks = this->encoder->getTicks(RIGHT);
	int leftTicks = this->encoder->getTicks(LEFT);
	this->encoder->clear();
	Position* positionChangeVector = calculatePositionChange(leftTicks, rightTicks);
	this->currentPosition.X += positionChangeVector->X;
	this->currentPosition.Y += positionChangeVector->Y;
	this->currentPosition.Rotation += positionChangeVector->Rotation;
	delete positionChangeVector;
}

Position* Localization::calculatePositionChange(int leftTicks, int rightTicks) {
	Position* positionChange = new Position();
	double distanceTravelled = ((double) (leftTicks + rightTicks)) / 2;
	positionChange->X = distanceTravelled * cos(this->currentPosition->Rotation);
	positionChange->Y = distanceTravelled * sin(this->currentPosition->Rotation);
	positionChange->Rotation = round(((leftTicks - rightTicks) / TICKS_PER_REV * WHEEL_DIAMETER) / (2 * WHEELBASE) * 360);
	return positionChange;
}
