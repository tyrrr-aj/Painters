#include "RpPoint.h"

RpPoint::RpPoint()
{

}

RpPoint::RpPoint(double coordX, double coordY, double currentDist)
{
	this->coordX = coordX;
	this->coordY = coordY;
	this->currentDist = currentDist;
	this->orderNumber = -1;
}

RpPoint RpPoint::pickFirst(std::vector<RpPoint*>& points, int number)
{
	for (int i = 0; i < number; ++i)
	{
		points[i]->currentDist = points[i]->coordX * points[i]->coordX + 
			points[i]->coordY * points[i]->coordY;
	}
	double minDist = INT16_MAX;
	RpPoint result;
	for (int i = 0; i < number; ++i)
	{
		if (points[i]->currentDist < minDist)
		{
			minDist = points[i]->currentDist;
			result = *points[i];
		}
	}
	return result;
}

double RpPoint::calculateDist(RpPoint other)
{
	return (this->coordX - other.coordX) * (this->coordX - other.coordX) +
		(this->coordY - other.coordY) * (this->coordY - other.coordY);
}

const RpPoint RpPoint::operator=(const RpPoint point)
{
	this->coordX = point.coordX;
	this->coordY = point.coordY;
	this->currentDist = point.currentDist;

	this->orderNumber = point.orderNumber;
	this->covered = point.covered;
}