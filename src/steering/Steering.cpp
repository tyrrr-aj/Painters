#include "Steering.h"

Steering::Steering(Motors* motor, Localization* localization)
{
	this->motor = motor;
	this->localization = localization;
}

void Steering::rotateChassis(Position current_localization, double angle)
{	
	Vector desiredVector;
	desiredVector.X = current_localization.rotation.X * cos(angle) - current_localization.rotation.Y * sin(angle);
	desiredVector.Y = current_localization.rotation.X * sin(angle) + current_localization.rotation.Y * cos(angle);
	
	if (angle > 0)
	{
		motor->rightMotor(100);
		motor->leftMotor(-100);
	}
	else
	{
		motor->leftMotor(100);
		motor->rightMotor(-100);
	}
	
	while(localization->getCurrentPosition().rotation != desiredVector)
	{
		delay(5);
	}
	
	motor->stop();
}

void Steering::calculateRotation(Point point)
{
	Position current_localization = localization.getCurrentPosition();
	
	Point current_position(current_localization.X, current_localization.Y);
	Point current_direction(current_localization.rotation.X, current_localization.rotation.Y);
	Angle angle(current_position, current_direction, point);

	angle.solveCoefficients();
	double angle = angle.getAngle();

	rotateChassis(current_localization, angle);
}

void Steering::leadChassis(Point point)
{
	motor->drive(150);
	Position destination = {point.coordX, point.coordY};
	while(!localization->getCurrentPosition().hasSameCoordinates(destination)){
		delay(5);
	}
	
	motor->stop();
}

void Steering::driveTo(Point point)
{
	rotateChassis(point);
	leadChassis(point);
}

Steering Steering::operator=(Steering steering)
{
	this->chassis = steering.chassis;
}