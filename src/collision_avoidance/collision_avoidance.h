#ifndef COLLISION_AVOIDANCE_H
#define COLLISION_AVOIDANCE_H

#include <vector>

#include "../communication/ble_communicator.h"
#include "../steering/steering.h"
#include "../geometry/Point.h"
#include "../geometry/lines.h"

#define PRIVATE_RADIUS 10.0 // maximum distance between robots which causes collision, unit must be the same as in path file

namespace collision_avoidance {
	enum Robots {ME, PARTNER, NEITHER};
	enum State {DRIVING, WAITING, DRIVING_WHILE_PARTNER_WAITS};
}

enum ResponseToProposal(AGREE, REJECT);

class Collision_avoidance {
	public:
		Collision_avoidance();
		Collision_avoidance(BLE_communicator* communicator, Steering* steering, Localization* localization);
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
		State state;

		void reactToPartnersCourseChange(Point partners_position, Point partners_destination);
		void reactToCollisionSpottedMessage(Point partners_position, Point partners_destination);
		void reactToProposalResponse(ResponseToProposal response);
		void reactToFreeWayAnnouncement();

		bool checkIfWillCollide(Point partners_position, Point partners_destination);
		void signalCollision();
		Robot checkWhoShouldWait(int partner_number_of_steps_to_free_way);
		int calculateNumberOfStepsToFreeWay(Point partners_position, Point partners_destination);
		void stopToGiveWay();
		void moveToGiveWay(Point partners_position, Point partners_destination);
};

#endif