#ifndef STEERING_H
#define STEERING_H

#include "Angle.h"

#include "../route_planning/Point.h"
#include "../motors/motors.h"
#include "../localization/localization.h"

class Steering
{
public:
	Steering() {}
	Steering(Motors*, Localization*);

	void driveTo(Point point);
private:
	Localization* localization;
	Motors* motor;

	Vector* transitionVector;
	void calculateTransitionVector(Point point);
	
	Angle* angle;
	void calculateRotation();
	
	void rotateChassis();
	void leadChassis();
	
	void calculateRotation(Point point);	
};

#endif