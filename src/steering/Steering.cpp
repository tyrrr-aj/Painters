#include "Steering.h"

Steering::Steering(Chassis chassis)
{
	this->chassis = chassis;
}

void Steering::rotateChassis(Point point)
{
	Point current_position = chassis.getPosition();
	Point current_direction = chassis.getDirection();
	Angle angle(current_position, current_direction, point);

	angle.solveCoefficients();
	double angle_cosinus = angle.designateRotation();
	Direction direction = angle.getDirection();

	chassis.rotate(angle_cosinus, direction);
}

void Steering::leadChassis(Point point)
{
	double distance = chassis.getPosition().calculateDist(point);

	chassis.ride(distance);
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