#ifndef CONTROL_H
#define CONTROL_H

#include "../steering/Steering.h"
#include "../collision_avoidance/collision_avoidance.h"
#include "../geometry/Point.h"

#include <vector>


class Control
{
private:
	std::vector<Point*> path;
	Steering* steering;
	Collision_avoidance* collision_avoidance;
	void moveRobot(Point point);
public:
	Control(std::vector<Point*> path, Steering* oneRobot, Collision_avoidance* collision_avoidance);

	void accomplishTrace();
};

#endif