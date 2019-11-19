#pragma once
#include "Angle.h"

class Chassis
{
private:
	Point position;
	Point direction;
public:
	Chassis() {}
	Chassis(Point startingPosition, Point startingDirection);

	Point getPosition();
	Point getDirection();

	void rotate(double angle, Direction direction);
	void ride(double time);

	Chassis operator=(Chassis chassis);
};

