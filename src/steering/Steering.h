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
	void finishInterruptedTask(Point destination);
	
private:
	Localization* localization;
	PausableMotors* motor;
	Vector* desiredPosition;
	Vector* transitionVector;

	void calculateTransitionVector(Point point);
	void rotateChassis();
	void leadChassis();

	bool taskAlreadyDone; // necessary in case robot has been redirected to a different point and then finishInterruptedTask() was called
};

#endif