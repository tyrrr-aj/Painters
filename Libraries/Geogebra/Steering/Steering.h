#pragma once
#include "Chassis.h"

class Steering
{
private:
	Chassis chassis;

	void rotateChassis(Point point);
	void leadChassis(Point point);
public:
	Steering() {}
	Steering(Chassis chassis);

	void driveTo(Point point);

	Steering operator=(Steering steering);
};