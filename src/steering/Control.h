#ifndef CONTROL_H
#define CONTROL_H

#include "Steering.h"

#include <vector>


class Control
{
private:
	std::vector<Point*> path;
	Steering steering;
	void moveRobot(Steering steering, Point point);
public:
	Control(std::vector<Point*> path, Steering oneRobot);

	void accomplishTrace();
};

#endif