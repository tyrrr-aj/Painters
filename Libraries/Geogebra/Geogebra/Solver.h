#pragma once
#include "Point.h"
#include <vector>

class Solver
{
private:
	double X;
	double Y;
	double radius;
	int numberOfPts;
	std::vector<Point*> points;

	bool allCovered = false;
	int currentOrderNumber;
public:
	Solver();
	Solver(std::vector<Point*> points, int X, int Y, int N);

	Point* pickFirst(std::vector<Point*>& points);

	void pickArea(Point point, std::vector<Point*>& areaPoints);
	void setDistances(Point point);
	Point* findNext(Point* point, std::vector<Point*> areaPoints);

	void distributeOrder(Point point, std::vector<Point*>& areaPoints);
	void distributeOrder1(Point* point, std::vector<Point*>& areaPoints);

	void solve();
};
