#include "Angle.h"

Angle::Angle(Point firstPoint, const Point secondPoint, const Point thirdPoint)
	:
	startPoint(secondPoint), destPoint(thirdPoint)
{
	double xVector = secondPoint.coordX - firstPoint.coordX;
	double yVector = secondPoint.coordY - firstPoint.coordY;
	
	viewPoint.coordX = secondPoint.coordX + xVector;
	viewPoint.coordY = secondPoint.coordY + yVector;
}

double Angle::solveAngle()
{
	//Point in which robot stopped
	double xStart = startPoint.coordX;
	double yStart = startPoint.coordY;
	//Direction in which robot is looking at
	double xView = viewPoint.coordX;
	double yView = viewPoint.coordY;
	//Destination where robot wants to go
	double xDest = destPoint.coordX;
	double yDest = destPoint.coordY;

	//if(yView != yStart)
		aCoeffView = (yView - yStart) / (xView - xStart);
	//else

	//if (yDest != yStart)
		aCoeefDest = (yDest - yStart) / (xDest - xStart);
	//else


	double tangens = abs((aCoeefDest - aCoeffView) / (1 + aCoeffView * aCoeefDest));

	return tangens;
}