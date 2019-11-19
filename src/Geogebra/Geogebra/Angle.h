#pragma once
#include "Point.h"
class Angle
{
private:
	const Point startPoint;
	Point viewPoint;
	const Point destPoint;

	double aCoeffView;
	double aCoeefDest;
public:
	Angle();
	Angle(Point firstPoint, const Point secondPoint, const Point thirdPoint);

	double solveAngle();
};

