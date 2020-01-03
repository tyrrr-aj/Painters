#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <vector>

#include "../communication/ble_communicator.h"
#include "../steering/steering.h"
#include "../geometry/Point.h"

class Collision_avoidance {
	public:
		Collision_avoidance();
		Collision_avoidance(BLE_communicator* communicator, Steering* steering, Localization* localization);
		void init(std::vector<Point*> path);
		void nextDestination(std::vector<Point*>::iterator new_destination);
	
	private:
		BLE_communicator* communicator;
		Steering* steering;
		Localization* localization;
		std::vector<Point*> path;
		std::vector<Point*>::iterator current_destination;
};

#endif