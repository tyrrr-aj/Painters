#include "Chassis.h"

Chassis::Chassis(Point startingPosition, Point startingDirection)
{
	this->position = startingPosition;
	this->direction = startingDirection;
}

Point Chassis::getDirection()
{
	return this->direction;
}

Point Chassis::getPosition()
{
	return this->position;
}

void Chassis::rotate(double angle, Direction direction)
{

}

void Chassis::ride(double time)
{

}

Chassis Chassis::operator=(Chassis chassis)
{
	this->position = chassis.position;
	this->direction = chassis.direction;
}