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
	encoder = encoder;
	currentPosition.X = currentPosition.Y = currentPosition.Position.rotation = 0;
}

Position Localization::getCurrentPosition() {
	updatePosition();
	return currentPosition;
}

/*********************************************************************
All methods below are private.
*********************************************************************/

void Localization::updatePosition() {
	int rightTicks = encoder->getTicks(RIGHT);
	int leftTicks = encoder->getTicks(LEFT);
	encoder->clear();
	
	double distanceTravelled = ((double) (leftTicks + rightTicks)) / 2;
	currentPosition.X += distanceTravelled * currentPosition.rotation.X;
	currentPosition.Y += distanceTravelled * currentPosition.rotation.Y;
	
	double rotationAngleInRadians = ((rightTicks - leftTicks) / TICKS_PER_REV * WHEEL_DIAMETER) / WHEELBASE * 2 * M_PI;
	currentPosition.rotation.X = currentPosition.rotation.X * cos(rotationAngleInRadians) - currentPosition.rotation.Y * sin(rotationAngleInRadians);
	currentPosition.rotation.Y = currentPosition.rotation.X * sin(rotationAngleInRadians) + currentPosition.rotation.Y * cos(rotationAngleInRadians);
}
