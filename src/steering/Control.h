#pragma once
#include <vector>
#include "Steering.h"

class Control
{
private:
	std::vector<Point> path;
	Steering steering;
	void moveRobot(Steering steering, Point point);
public:
	Control(std::vector<Point> path, Steering oneRobot);

	void accomplishTrace();
};

