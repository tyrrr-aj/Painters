#ifndef STEERING_H
#define STEERING_H

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
	Vector* desiredPosition;
	Vector* transitionVector;
	
	void calculateTransitionVector(Point point);
	void rotateChassis();
	void leadChassis();
};

#endif