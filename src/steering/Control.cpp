#include "Control.h"

Control::Control(std::vector<Point*> path, Steering oneRobot)
{
	this->path = path;
	this->steering = oneRobot;
}

void Control::moveRobot(Steering steering, Point point)
{
	steering.driveTo(point);
}

void Control::accomplishTrace()
{
	for (int i = 0; i < path.size(); ++i)
	{
		moveRobot(steering, *path[i]);
	}
}