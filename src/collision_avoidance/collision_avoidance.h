#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <vector>

#include "../communication/ble_communicator.h"
#include "../steering/steering.h"
#include "../geometry/Point.h"
#include "../geometry/lines.h"

#define PRIVATE_RADIUS 10.0 // maximum distance between robots which causes collision, unit must be the same as in path file

class BLE_communicator;

namespace collision_avoidance {
	enum Robot {ME, PARTNER, NEITHER};
	enum State {DRIVING, WAITING, DRIVING_WHILE_PARTNER_WAITS};
}

 class Collision_avoidance {
 	public:
	  	Collision_avoidance();
		Collision_avoidance(Steering* steering, Localization* localization);
		
		void addCommunicator(BLE_communicator* communicator);
		void init(std::vector<Point*> path);
		void newDestination(std::vector<Point*>::iterator new_destination);
		void destinationReached();
	
	private:
		BLE_communicator* communicator;
		Steering* steering;
		Localization* localization;
		std::vector<Point*> path;
		std::vector<Point*>::iterator current_destination;
		int steps_to_free_way;
		collision_avoidance::State state;

		Point partners_position;
		Point partners_destination;

		void reactToPartnersCourseChange(Point partners_position, Point partners_destination);
		void reactToCollisionSpottedMessage(Point partners_position, Point partners_destination);
		void reactToProposal(int partners_number_of_steps_to_free_way);
		void reactToProposalResponse(ResponseToProposal response);
		void reactToFreeWayAnnouncement();

		bool checkIfPathsAreCrossing(Point first_robot_position, Point first_robot_destination, Point second_robot_position, Point second_robot_destination);
		bool checkForDirectCollision(Point moving_robot_position, Point moving_robot_destination, Point second_robot_position);
	
		void signalCollision();
		collision_avoidance::Robot checkWhoShouldWait(int partner_number_of_steps_to_free_way);
		int calculateNumberOfStepsToFreeWay(Point partners_position, Point partners_destination);
		void stopToGiveWay();
		void moveToGiveWay(Point partners_position, Point partners_destination);
};

#endif