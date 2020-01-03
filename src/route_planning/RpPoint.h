#ifndef RP_POINT_H
#define RP_POINT_H

#include <iostream>
#include <vector>
#include <limits.h>

class RpPoint
{
public:
	double coordX;
	double coordY;
	
	double currentDist;
	int orderNumber;

	bool covered;


	RpPoint();
	RpPoint(double coordX, double coordY, double currentDist = 0);

	RpPoint pickFirst(std::vector<RpPoint*>& points, int number);
	double calculateDist(RpPoint other);

	//Point operator=(Point point);
	const RpPoint operator=(const RpPoint point);
};

#endif