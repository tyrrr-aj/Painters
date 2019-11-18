#pragma once
#include "Point.h"
#include "Vector.h"

enum Direction
{
	LEFT,
	RIGHT
};

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
	Direction getDirection();

	Angle operator=(Angle angle);
};

