#ifndef STEERING_H
#define STEERING_H

#include "../geometry/Point.h"
#include "../motors/pausable_motors.h"
#include "../localization/localization.h"

#define BASE_SPEED 100

class Steering
{
public:
	Steering() {}
	Steering(PausableMotors*, Localization*);

	void driveTo(Point point);

	void pause();
	void resume();
	
private:
	Localization* localization;
	PausableMotors* motor;
	Vector* desiredPosition;
	Vector* transitionVector;

	void calculateTransitionVector(Point point);
	void rotateChassis();
	void leadChassis();
};

#endif