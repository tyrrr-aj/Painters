#include "collision_avoidance.h"
#include "../communication/ble_communicator.h"

using namespace collision_avoidance;

Collision_avoidance::Collision_avoidance() {}

Collision_avoidance::Collision_avoidance(Steering* steering, Localization* localization) {
	this->steering = steering;
	this->localization = localization;
}

void Collision_avoidance::addCommunicator(BLE_communicator* communicator) {
    this->communicator = communicator;
}

void Collision_avoidance::init(std::vector<Point*> path) {
	communicator->setUpCommunication();
	this->path = path;
    this->current_destination = path.begin();
}

void Collision_avoidance::newDestination(std::vector<Point*>::iterator new_destination) {
	Point current_position = Point(localization->getCurrentXY());
	communicator->announceNewCourse(current_position, **new_destination);
}

void Collision_avoidance::destinationReached() {
	if (state == DRIVING_WHILE_PARTNER_WAITS) {
		steps_to_free_way--;
		if (steps_to_free_way == 0) {
			state = DRIVING;
			communicator->announceFreeWay();
		}
	}
}


/*********************** CALLBACKS ****************************/


void Collision_avoidance::reactToPartnersCourseChange(Point partners_position, Point partners_destination) {
	if (checkIfPathsAreCrossing(Point(localization->getCurrentXY()), **current_destination, partners_position, partners_destination)) {
        this->partners_position = partners_position;
        this->partners_destination = partners_destination;
        signalCollision();
	}
}

void Collision_avoidance::reactToCollisionSpottedMessage(Point partners_position, Point partners_destination) {
	communicator->propose(calculateNumberOfStepsToFreeWay(partners_position, partners_destination));
}

void Collision_avoidance::reactToProposal(int partners_number_of_steps_to_free_way) {
	switch (checkWhoShouldWait(partners_number_of_steps_to_free_way)) {
		case ME:
			communicator->respondToProposal(protocol::ResponseToProposal::AGREE);
			stopToGiveWay();
			break;
		case PARTNER:
			communicator->respondToProposal(protocol::ResponseToProposal::REJECT);
			state = DRIVING_WHILE_PARTNER_WAITS;
			break;
		case NEITHER:
			communicator->respondToProposal(protocol::ResponseToProposal::REJECT);
			moveToGiveWay(partners_position, partners_destination);
			communicator->announceFreeWay();
	}
}

void Collision_avoidance::reactToProposalResponse(protocol::ResponseToProposal response) {
	switch (response) {
		case protocol::ResponseToProposal::AGREE:
			state = DRIVING_WHILE_PARTNER_WAITS;
			break;
		case protocol::ResponseToProposal::REJECT:
			stopToGiveWay();
			break;
	}
}

void Collision_avoidance::reactToFreeWayAnnouncement() {
	state = DRIVING;
	steering->resume();
}


/******************** PRIVATE METHODS *************************/


bool Collision_avoidance::checkIfPathsAreCrossing(Point first_robot_position, Point first_robot_destination, Point second_robot_position, Point second_robot_destination) {
	return lines::getDistanceBetweenLines(first_robot_position, first_robot_destination, second_robot_position, partners_destination) < PRIVATE_RADIUS;
}

bool Collision_avoidance::checkForDirectCollision(Point moving_robot_position, Point moving_robot_destination, Point second_robot_position) {
	return lines::getDistanceBetweenPointAndLine(second_robot_position, moving_robot_position, moving_robot_destination) < PRIVATE_RADIUS;
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
	int number_of_steps;
	std::vector<Point*>::iterator pos;
	std::vector<Point*>::iterator des;
	for (number_of_steps = 1,
			pos = this->current_destination,
			des = std::next(this->current_destination, 1);
			des != path.end();
			pos++, des++, number_of_steps++) {
		if (checkForDirectCollision(**pos, **des, partners_position)) {
			return -1;
		}
		if (!checkIfPathsAreCrossing(**pos, **des, partners_position, partners_destination)) {
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
	/*Point bypass_point = lines.findOptimalBypassPoint(Point(localization->getCurrentXY), **current_destination, partners_position, partners_destination)
	steering->driveTo(bypass_point);
	communicator->announceFreeWay();
	steering->finishInterruptedTask(**current_destination);*/
    steering->pause();
}