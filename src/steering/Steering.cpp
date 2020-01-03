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
	
//	Position currentPosition = localization->getCurrentPosition();
//	currentVector = new Vector(currentPosition.position.X, currentPosition.position.Y);
	Vector currentXY = localization->getCurrentXY();
	currentVector = new Vector(currentXY.X, currentXY.Y);
	
	double desiredX = point.X - currentVector->X;
	double desiredY = point.Y - currentVector->Y;
	transitionVector = new Vector(desiredX, desiredY);
}

void Steering::calculateRotation()
{
	/*******************************
	Create angle object of needed rotation
	*******************************/
	
//	Position currentPosition = localization->getCurrentPosition();
//	Vector currentRotation = currentPosition.rotation;
	Vector currentRotation = localization->getCurrentRotation();
	angle = new Angle (*transitionVector, currentRotation);
}

/*********************************************************************
Methods which PRACTICALLY are moving chassis
*********************************************************************/

void Steering::rotateChassis()
{	
	double rotationAngle = angle->getRotation();
	
	Serial.print("rotationAngle: ");
	Serial.println(rotationAngle);
	
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
	
	Vector rot = localization->getCurrentRotation();
	double previousDistance = desiredRotation.distance(rot);
	
	Serial.print("currentRotation: ");
	Serial.print(rot.X);
	Serial.print(" ");
	Serial.println(rot.Y);
	Serial.print("desiredRotation: ");
	Serial.print(desiredRotation.X);
	Serial.print(" ");
	Serial.println(desiredRotation.Y);
	
//	while((rot = localization->getCurrentPosition().rotation) != desiredRotation)
	while(desiredRotation.distance(rot) <= previousDistance) {
		previousDistance = desiredRotation.distance(rot);
		rot = localization->getCurrentRotation();
//	while((rot = localization->getCurrentRotation()) != desiredRotation)
//	{
		Serial.print("currentRotation: ");
		Serial.print(rot.X);
		Serial.print(" ");
		Serial.println(rot.Y);
		delay(100);
	}
	
	motor->stop();
}

void Steering::leadChassis()
{
	double desiredX = transitionVector->X - currentVector->X;
	double desiredY = transitionVector->Y - currentVector->Y;
	Vector desiredPosition(desiredX, desiredY);
	
	motor->drive(150);
	
	Serial.print("desired position: ");
	Serial.print(desiredX);
	Serial.print(", ");
	Serial.println(desiredY);
	
	Vector loc = localization->getCurrentXY();
	
	double previousDistance = desiredPosition.distance(loc);
	
//	while((loc = localization->getCurrentPosition().position) != desiredPosition){
	while(desiredPosition.distance(loc) <= previousDistance) {
		previousDistance = desiredPosition.distance(loc);
		loc = localization->getCurrentXY();
		Serial.print("current position: ");
		Serial.print(loc.X);
		Serial.print(", ");
		Serial.println(loc.Y);
		delay(100);
	}
	
	motor->stop();
}