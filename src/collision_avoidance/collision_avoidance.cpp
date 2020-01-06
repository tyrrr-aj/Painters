#include "collision_avoidance.h"

using namespace collision_avoidance;

Collision_avoidance::Collision_avoidance() {}

Collision_avoidance::Collision_avoidance(BLE_communicator* communicator, Steering* steering, Localization* localization) {
	this->communicator = communicator;
	this->steering = steering;
	this->localization = localization;
}

void Collision_avoidance::init(std::vector<Point*> path) {
	communicator->setUpCommunication();
	communicator->registerNewCourseCallback(reactToPartnersCourseChange);
	communicator->registerCollisionSpottedCallback(reactToCollisionSpottedMessage);
	communicator->registerGiveWayCallback(reactToGiveWayMessage);
	this->path = path;
}

void Collision_avoidance::newDestination(std::vector<Point*>::iterator new_destination) {
	Point current_position = Point(localization->getCurrentXY());
	communicator->announceNewCourse(current_position, **new_destination);
}

void Collision_avoidance::destinationReached() {
	if (robot_state == DRIVING_WHILE_PARTNER_WAITS) {
		steps_to_free_way--;
		if (steps_to_free_way == 0) {
			state = DRIVING;
			communicator->announceFreeWay();
		}
	}
}

/*********************** CALLBACKS ****************************/

void Collision_avoidance::reactToPartnersCourseChange(Point partners_position, Point partners_destination) {
	if (checkIfWillCollide(Point(localization->getCurrentXY()), **current_destination, partners_position, partners_destination)) {
		signalCollision();
		int partners_number_of_steps_to_free_way = communicator->waitForProposal();
		switch (checkWhoShouldWait(partners_number_of_steps_to_free_way)) {
			case ME:
				communicator->respondToProposal(AGREE);
				stopToGiveWay();
				break;
			case PARTNER:
				communicator->respondToProposal(REJECT);
				state = DRIVING_WHILE_PARTNER_WAITS;
				break;
			case NEITHER:
				communicator->respondToProposal(REJECT);
				moveToGiveWay(partners_position, partners_destination);
				communicator->announceFreeWay();
		}
	}
}

void Collision_avoidance::reactToCollisionSpottedMessage(Point partners_position, Point partners_destination) {
	communicator->propose(calculateNumberOfStepsToFreeWay(partners_position, partners_destination));
}

void Collision_avoidance::reactToProposalResponse(ResponseToProposal response) {
	switch (response) {
		case AGREE:
			state = DRIVING_WHILE_PARTNER_WAITS;
			break;
		case REJECT:
			stopToGiveWay();
			break;
	}
}

void Collision_avoidance::reactToFreeWayAnnouncement() {
	state = DRIVING;
	steering->resume();
}

/******************** PRIVATE METHODS *************************/

bool Collision_avoidance::checkIfWillCollide(Point first_robot_position, Point first_robot_destination, Point second_robot_position, Point second_robot_destination) {
	return lines::getDistance(first_robot_position, first_robot_destination, second_robot_position, partners_destination) < PRIVATE_RADIUS;
}

void Collision_avoidance::signalCollision() {
	Point current_position = Point(localization->getCurrentXY());
	communicator->signalCollision(current_position, **current_destination);
}

Robot Collision_avoidance::checkWhoShouldWait(int partners_number_of_steps_to_free_way) {
	int own_number_of_steps_to_free_way = calculateNumberOfStepsToFreeWay(partners_position, partners_destination);
	if (own_number_of_steps_to_free_way == -1 && partners_number_of_steps_to_free_way == -1) {
			return NEITHER;
		}
		else if (own_number_of_steps_to_free_way > partners_number_of_steps_to_free_way) {
			return ME;
		}
		else {
			steps_to_free_way = own_number_of_steps_to_free_way;
			return PARTNER;
		}
}

int Collision_avoidance::calculateNumberOfStepsToFreeWay(Point partners_position, Point partners_destination) {
	for (int number_of_steps = 1,
			std::vector<Point*>::iterator pos = current_destination;
			std::vector<Point*>::iterator des = std::next(current_destination, 1);
			des != path.end();
			pos++, des++, number_of_steps++) {
		if (!checkIfWillCollide(**pos, **des, partners_position, partners_destination)) {
			return number_of_steps;
		}
	}
	return -1;
}

void Collision_avoidance::stopToGiveWay() {
	state = WAITING;
	steering->pause();
}

void Collision_avoidance::moveToGiveWay(Point partners_position, Point partners_destination) {
	lines.findOptimalBypassPoint(Point(localization->getCurrentXY), **current_destination, partners_position, partners_destination)
}