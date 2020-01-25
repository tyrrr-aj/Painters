#include "Solver.h"

Solver::Solver()
{
}

Solver::Solver(std::vector<RpPoint*> points, int X, int Y, int N)
{
	this->X = X;
	this->Y = Y;
	#define R (X * X + Y * Y) / 2000
	this->radius = 0.19;//(X * X + Y * Y) / 2000;

	this->numberOfPts = N;
	for (int i = 0; i < N; ++i)
	{
		this->points.push_back(points[i]);
	}
}

RpPoint* Solver::pickFirst(std::vector<RpPoint*>& points)
{
	for (int i = 0; i < numberOfPts; ++i)
	{
		points[i]->currentDist = points[i]->coordX * points[i]->coordX +
			points[i]->coordY * points[i]->coordY;
	}
	double minDist = INT16_MAX;
	RpPoint* result = NULL;
	for (int i = 0; i < numberOfPts; ++i)
	{
		if (points[i]->currentDist < minDist)
		{
			minDist = points[i]->currentDist;
			result = points[i];
		}
	}
	return result;
}

void Solver::setDistances(RpPoint point)
{
	for (int i = 0; i < numberOfPts; ++i)
	{
		double xDiff = points[i]->coordX - point.coordX;
		double yDiff = points[i]->coordY - point.coordY;
		points[i]->currentDist = xDiff * xDiff + yDiff * yDiff;
	}
}

void Solver::pickArea(RpPoint point, std::vector<RpPoint*>& areaPoints)
{
	for (int i = 0; i < numberOfPts; ++i)
	{
		if (points[i]->currentDist < radius && points[i]->orderNumber < 0)
		{
			areaPoints.push_back(this->points[i]);
		}
	}
	if (!areaPoints.size())
	{
		radius *= 1.3;
		pickArea(point, areaPoints);
	}
}

RpPoint* Solver::findNext(RpPoint* point, std::vector<RpPoint*> areaPoints)
{
	double maxDist = INT_MAX;
	RpPoint* nextPoint = new RpPoint();
	int pos = -1;
	for (int p = 0; p != areaPoints.size(); ++p)
	{
		double distance = point->calculateDist(*areaPoints[p]);
		if (distance < maxDist)
		{
			nextPoint = areaPoints[p];
			pos = p;
			maxDist = distance;
		}
	}
	return nextPoint;
}

void Solver::distributeOrder(RpPoint point, std::vector<RpPoint*>& areaPoints)
{
	RpPoint* currentpoint = &point;
	for(int i=0; i < areaPoints.size(); ++i)
	{
		currentpoint = findNext(currentpoint, areaPoints);
		currentpoint->orderNumber = currentOrderNumber;
		++currentOrderNumber;
	}
}

void Solver::distributeOrder1(RpPoint* point, std::vector<RpPoint*>& areaPoints)
{
	point = findNext(point, areaPoints);
	point->orderNumber = currentOrderNumber;
	++currentOrderNumber;
	if (currentOrderNumber >= this->points.size()) allCovered = true;
}

void Solver::solve()
{
	RpPoint* currentPoint = pickFirst(points);
	currentPoint->orderNumber = 0;
	currentOrderNumber = 1;
	while (!allCovered)
	{
		std::vector<RpPoint*> areaPoints;
		setDistances(*currentPoint);
		pickArea(*currentPoint, areaPoints);
		distributeOrder1(currentPoint, areaPoints);
	}
}