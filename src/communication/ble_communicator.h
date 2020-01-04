#ifndef BLE_COMMUNICATOR_H
#define BLE_COMMUNICATOR_H

#include "../geometry/Point.h"
#include "../collision_avoidance/collision_avoidance.h"

class BLE_communicator {
    public:
        BLE_communicator();
        void setUpCommunication();

        void registerNewCourseCallback(void (Collision_avoidance::*callback)(Point, Point));
        void registerCollisionSpottedCallback(void (Collision_avoidance::*callback)(Point, Point));
        void registerGiveWayCallback(void (Collision_avoidance::*callback)());
        void registerFreeWayCallback(void (Collision_avoidance::*callback)());

        void announceNewCourse(Point position, Point destination);
        void signalCollision(Point ownPosition, Point ownDestination);
        void propose(int number_of_steps);
        int waitForProposal();
        void announceFreeWay();
        void respondToProposal(ResponseToProposal response);
};

#endif