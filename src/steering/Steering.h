#pragma once
#include "../motors/motors.h"
#include "../localization/localization.h"
#include "Angle.h"
#include "../vectors/Vector.h"

class Steering
{
private:
	Localization* localization;
	Motors* motor;
	
	void rotateChassis(Position current_localization, double angle);
	void leadChassis(Point point);
	
	void calculateRotation(Point point);
public:
	Steering() {}
	Steering(Motors*, Localization*);

	void driveTo(Point point);

	Steering operator=(Steering steering);
};