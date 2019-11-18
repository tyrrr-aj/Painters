#include "Angle.h"

Angle::Angle()
{

}

Angle::Angle(Point startPoint, const Point viewPoint, const Point destPoint)
	:
	startPoint(startPoint), viewPoint(viewPoint), destPoint(destPoint)
{
}

void Angle::solveCoefficients()
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

	viewVector.setVector(xView - xStart, yView - yStart);
	destVector.setVector(xDest - xStart, yDest - yStart);

	//if (yView != yStart)
	aCoeffView = (yView - yStart) / (xView - xStart);
	//else

	//if (yDest != yStart)
	aCoeffDest = (yDest - yStart) / (xDest - xStart);
	//else
}

double Angle::designateRotation()
{
	//return abs((aCoeffDest - aCoeffView) / (1 + aCoeffView * aCoeffDest));
	return viewVector.getX() * destVector.getX() + viewVector.getY() * destVector.getY();
}

Direction Angle::getDirection()
{
	if (viewVector.getX() * destVector.getY() - viewVector.getY() * destVector.getX() > 0)
		return RIGHT;
	else
		return LEFT;
}

Angle Angle::operator=(Angle angle)
{
	this->startPoint = angle.startPoint;
	this->viewPoint = angle.viewPoint;
	this->destPoint = angle.destPoint;

	this->aCoeffView = angle.aCoeffView;
	this->aCoeffDest = angle.aCoeffDest;
}