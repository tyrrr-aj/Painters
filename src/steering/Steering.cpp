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
	
	Position currentPosition = localization->getCurrentPosition();
	currentVector = new Vector(currentPosition.position.X, currentPosition.position.Y);
	
	double desiredX = point.coordX - currentVector->X;
	double desiredY = point.coordY - currentVector->Y;
	transitionVector = new Vector(desiredX, desiredY);
}

void Steering::calculateRotation()
{
	/*******************************
	Create angle object of needed rotation
	*******************************/
	
	Position currentPosition = localization->getCurrentPosition();
	Vector currentRotation = currentPosition.rotation;
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
		motor->leftMotor(100);
		motor->rightMotor(-100);
	}
	
	Vector desiredRotation = transitionVector->getNormalVector();
	
	while(localization->getCurrentPosition().rotation != desiredRotation)
	{
		delay(5);
	}
	
	motor->stop();
}

void Steering::leadChassis()
{
	double desiredX = transitionVector->X - currentVector->X;
	double desiredY = transitionVector->Y - currentVector->Y;
	Vector desiredPosition(desiredX, desiredY);
	
	motor->drive(150);
	
	while(localization->getCurrentPosition().position != desiredPosition){
		delay(5);
	}
	
	motor->stop();
}