#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <limits.h>

#include "../geometry/Point.h"

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

	Point* pickFirst(std::vector<Point*>& points);

	void pickArea(Point point, std::vector<Point*>& areaPoints);
	void setDistances(Point point);
	Point* findNext(Point* point, std::vector<Point*> areaPoints);

	void distributeOrder(Point point, std::vector<Point*>& areaPoints);
	void distributeOrder1(Point* point, std::vector<Point*>& areaPoints);
public:
	Solver();
	Solver(std::vector<Point*> points, int X, int Y, int N);

	void solve();
};

#endif