#pragma once
#include "../route_planning/Point.h"
#include "../vectors/Vector.h"
#include <math.h> 

class Angle
{
private:
	Point startPoint;
	Point viewPoint;
	Point destPoint;

	double aCoeffView;
	double aCoeffDest;

	Vector viewVector;
	Vector destVector;
public:
	Angle();
	Angle(Point startPoint, const Point viewPoint, const Point destPoint);

	void solveCoefficients();
	double designateRotation();
	bool shouldRotateRight();

	Angle operator=(Angle angle);
};

