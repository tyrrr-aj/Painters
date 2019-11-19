#pragma once
#include <iostream>
#include <vector>

class Point
{
public:
	double coordX;
	double coordY;
	
	double currentDist;
	int orderNumber;

	bool covered;


	Point();
	Point(double coordX, double coordY, double currentDist = 0);

	Point pickFirst(std::vector<Point*>& points, int number);
	double calculateDist(Point other);

	//Point operator=(Point point);
	const Point operator=(const Point point);
};
