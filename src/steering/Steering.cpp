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
	double desiredX = point.coordX - currentPosition.position.X;
	double desiredY = point.coordY - currentPosition.position.Y;
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
	/////////////////////////TODOOO
	/*motor->drive(150);
	Position destination = {point.coordX, point.coordY};
	while(!localization->getCurrentPosition().hasSameCoordinates(destination)){
		delay(5);
	}
	
	motor->stop();*/
}