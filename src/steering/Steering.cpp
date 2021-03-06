#include "Steering.h"

Steering::Steering(PausableMotors* motor, Localization* localization)
{
	this->motor = motor;
	this->localization = localization;
}

void Steering::driveTo(Point point)
{
	calculateTransitionVector(point);

	rotateChassis();
	delay(1200);
	
	leadChassis();
	delay(1200);
}

void Steering::pause() {
	motor->pause();
}

void Steering::resume() {
	motor->resume();
}

/*********************************************************************
All methods below are private.
*********************************************************************/

/*********************************************************************
Methods to calculate needed PARAMETERES to move chassis
*********************************************************************/

void Steering::calculateTransitionVector(Point point)
{
	/*******************************
	Create vector of desired transition
	(Point A ----> Point B)
	*******************************/
	
	desiredPosition = new Vector(point.X, point.Y);
	
	Vector currentXY = localization->getCurrentXY();
	double xTransition = desiredPosition->X - currentXY.X;
	double yTransition = desiredPosition->Y - currentXY.Y;
	transitionVector = new Vector(xTransition, yTransition);
}

/*********************************************************************
Methods which PRACTICALLY are moving chassis
*********************************************************************/

void Steering::rotateChassis()
{
	Vector currentRotation = localization->getCurrentRotation();
	Vector desiredRotation = transitionVector->getNormalVector();
	
	//Resolving turning direction by using vector product
	double vectorProduct = currentRotation.X * desiredRotation.Y
		- currentRotation.Y * desiredRotation.X;

	if (vectorProduct >= 0)
	{
		//Turn chassis anticlockwise
		motor->rightMotor(BASE_SPEED);
		motor->leftMotor(-BASE_SPEED);
	}
	else
	{
		//Turn chassis clockwise
		motor->leftMotor(BASE_SPEED);
		motor->rightMotor(-BASE_SPEED);
	}

	//double previousDistance = desiredRotation.distance(currentRotation);

	while (currentRotation != desiredRotation) {
		delay(30);
	}

	motor->stop();
	delay(100);
	localization->clear();
}

void Steering::leadChassis()
{
	Vector currentXY = localization->getCurrentXY();

	motor->drive(BASE_SPEED);

	double previousDistance = desiredPosition->distance(currentXY);
	
	while (desiredPosition->distance(currentXY) <= previousDistance)
	{
		previousDistance = desiredPosition->distance(currentXY);
		currentXY = localization->getCurrentXY();
		delay(100);
	}

	motor->stop();
	delay(100);
	localization->clear();
}