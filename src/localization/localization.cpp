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

Localization::Localization(Encoder* encoder) {
	this->encoder = encoder;
	currentPosition.position.X = 0.0;
	currentPosition.position.Y = 0.0;
	currentPosition.rotation.X = 1;
}

Position Localization::getCurrentPosition() {
	updatePosition();
	return currentPosition;
}

Vector Localization::getCurrentXY() {
	int rightTicks = encoder->getTicks(RIGHT);
	int leftTicks = encoder->getTicks(LEFT);
	Serial.print("TICKS: right = ");
	Serial.print(rightTicks);
	Serial.print(", left = ");
	Serial.println(leftTicks);
	Serial.print("currentRotation: ");
	Serial.print(currentPosition.rotation.X);
	Serial.print(", ");
	Serial.println(currentPosition.rotation.Y);
	encoder->clear();
	updateXY(leftTicks, rightTicks);
	return currentPosition.position;
}

Vector Localization::getCurrentRotation() {
	int rightTicks = encoder->getTicks(RIGHT);
	int leftTicks = encoder->getTicks(LEFT);
	Serial.print("TICKS: right = ");
	Serial.print(rightTicks);
	Serial.print(", left = ");
	Serial.println(leftTicks);
	encoder->clear();
	updateRotation(leftTicks, rightTicks);
	return currentPosition.rotation;
}

/*********************************************************************
All methods below are private.
*********************************************************************/

void Localization::updatePosition() {
	int rightTicks = encoder->getTicks(RIGHT);
	int leftTicks = encoder->getTicks(LEFT);
	Serial.print("TICKS: right = ");
	Serial.print(rightTicks);
	Serial.print(", left = ");
	Serial.println(leftTicks);
	encoder->clear();
	updateXY(leftTicks, rightTicks);
	updateRotation(leftTicks, rightTicks);
}
	
void Localization::updateXY(int leftTicks, int rightTicks) {	
	double distanceTravelled = (((double) (leftTicks + rightTicks)) / (2 * TICKS_PER_REV)) * (WHEEL_DIAMETER * M_PI);
	double xTransition = distanceTravelled * currentPosition.rotation.X;
	double yTransition = distanceTravelled * currentPosition.rotation.Y;

	currentPosition.position.X += xTransition;
	currentPosition.position.Y += yTransition;
}

void Localization::updateRotation(int leftTicks, int rightTicks) {
	double rotationAngleInRadians = ((double) (abs(rightTicks) + abs(leftTicks)) / (2 * TICKS_PER_REV) * WHEEL_DIAMETER) / WHEELBASE * 2 * M_PI;
	rotationAngleInRadians = rightTicks > leftTicks ? rotationAngleInRadians : -rotationAngleInRadians;
//	Serial.print("angle: ");
//	Serial.println(rotationAngleInRadians);
	double xRotation = currentPosition.rotation.X * cos(rotationAngleInRadians) - currentPosition.rotation.Y * sin(rotationAngleInRadians);
	double yRotation = currentPosition.rotation.X * sin(rotationAngleInRadians) + currentPosition.rotation.Y * cos(rotationAngleInRadians);
	
	currentPosition.rotation.X = xRotation;
	currentPosition.rotation.Y = yRotation;
}
