#include "Steering.h"

Steering::Steering(Motors* motor, Localization* localization)
{
	this->motor = motor;
	this->localization = localization;
}

void Steering::driveTo(Point point)
{
	calculateTransitionVector(point);
	calculateRotation();
	
	rotateChassis();
	leadChassis();
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
	
	Vector currentXY = localization->getCurrentXY();
	currentVector = new Vector(currentXY.X, currentXY.Y);
	
	double desiredX = point.coordX - currentVector->X;
	double desiredY = point.coordY - currentVector->Y;
	transitionVector = new Vector(desiredX, desiredY);
}

void Steering::calculateRotation()
{
	/*******************************
	Create angle object of needed rotation
	*******************************/

	Vector currentRotation = localization->getCurrentRotation();
	angle = new Angle (*transitionVector, currentRotation);
}

/*********************************************************************
Methods which PRACTICALLY are moving chassis
*********************************************************************/

void Steering::rotateChassis()
{	
	double rotationAngle = angle->getRotation();
	
	if (rotationAngle > 0)
	{
		motor->rightMotor(100);
		motor->leftMotor(-100);
	}
	else
	{
		motor->rightMotor(-100);
		motor->leftMotor(100);
	}
	
	Vector desiredRotation = transitionVector->getNormalVector();
	
	Vector rot = localization->getCurrentRotation();
	double previousDistance = desiredRotation.distance(rot);
	
	while((rot = localization->getCurrentRotation()) != desiredRotation)
	{
		delay(100);
	}
	
	motor->stop();
	delay(100);
	localization->clear();
}

void Steering::leadChassis()
{
	double desiredX = transitionVector->X - currentVector->X;
	double desiredY = transitionVector->Y - currentVector->Y;
	Vector desiredPosition(desiredX, desiredY);
	
	motor->drive(150);
	
	Vector loc = localization->getCurrentXY();
	
	double previousDistance = desiredPosition.distance(loc);

	while(desiredPosition.distance(loc) <= previousDistance) {
		previousDistance = desiredPosition.distance(loc);
		loc = localization->getCurrentXY();
		Serial.print(loc.X);
		Serial.print(", ");
		Serial.println(loc.Y);
		delay(100);
	}
	
	motor->stop();
	delay(100);
	localization->clear();
}