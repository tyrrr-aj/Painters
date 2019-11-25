#ifndef ANGLE_H
#define ANGLE_H

#include "../localization/Vector.h"

#include <math.h> 

namespace angle_direction
{
	enum DIRECTION {LEFT,RIGHT};
}

class Angle
{
public:
	Angle();
	Angle(Vector,Vector);

	double getRotation();

	Angle operator=(Angle angle);
private:
	Vector desiredTransition;
	Vector rotation;

	double rotationCosinus();
	angle_direction::DIRECTION rotationDirection();
};

#endif