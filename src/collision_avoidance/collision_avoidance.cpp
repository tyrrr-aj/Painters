#include "collision_avoidance.h"

Collision_avoidance::Collision_avoidance() {}

Collision_avoidance::Collision_avoidance(BLE_communicator* communicator, Steering* steering, Localization* localization) {
	this->communicator = communicator;
	this->steering = steering;
	this->localization = localization;
}

void Collision_avoidance::init(std::vector<Point*> path) {
	communicator->setUpCommunication();
	//communicator->registerCallback();
	this->path = path;
}

void Collision_avoidance::nextDestination(std::vector<Point*>::iterator new_destination) {
	Point current_position = Point(localization->getCurrentXY());
	communicator->announceDirection(current_position, **new_destination);
}