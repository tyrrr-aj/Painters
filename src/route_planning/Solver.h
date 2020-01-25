#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <limits.h>

#include "RpPoint.h"

class Solver
{
private:
	double X;
	double Y;
	double radius;
	int numberOfPts;
	std::vector<RpPoint*> points;

	bool allCovered = false;
	int currentOrderNumber;

	RpPoint* pickFirst(std::vector<RpPoint*>& points);

	void pickArea(RpPoint point, std::vector<RpPoint*>& areaPoints);
	void setDistances(RpPoint point);
	RpPoint* findNext(RpPoint* point, std::vector<RpPoint*> areaPoints);

	void distributeOrder(RpPoint point, std::vector<RpPoint*>& areaPoints);
	void distributeOrder1(RpPoint* point, std::vector<RpPoint*>& areaPoints);
public:
	Solver();
	Solver(std::vector<RpPoint*> points, int X, int Y, int N);

	void solve();
};

#endif