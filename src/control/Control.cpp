#include "Control.h"

Control::Control(std::vector<Point*> path, Steering* oneRobot, Collision_avoidance* collision_avoidance)
{
	this->path = path;
	this->steering = oneRobot;
	this->collision_avoidance = collision_avoidance;
}

void Control::moveRobot(Point point)
{
	steering->driveTo(point);
}

void Control::accomplishTrace()
{
	collision_avoidance->init(path);
	for (std::vector<Point*>::iterator current_dest = path.begin(); current_dest != path.end(); ++current_dest)
	{
		collision_avoidance->newDestination(current_dest);
		moveRobot(**current_dest);
		collision_avoidance->destinationReached();
	}
}