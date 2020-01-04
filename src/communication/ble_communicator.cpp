#include "ble_communicator.h"

BLE_communicator::BLE_communicator() {

}

void BLE_communicator::setUpCommunication() {
    // create servers and clients, establish connections
}

void BLE_communicator::registerNewCourseCallback(void (Collision_avoidance::*callback)(Point, Point)) {
    // make sure that provided function will be called each time partner invokes his announceNewCourse()
}

void BLE_communicator::announceNewCourse(Point position, Point destination) {
    // pass provided coordinates to partner, partner should invoke it's newCourseCalback()
}

void BLE_communicator::registerCollisionSpottedCallback(void (Collision_avoidance::*callback)(Point, Point)) {
    // make sure that provided function will be called each time partner invokes his signalCollision()
}

void BLE_communicator::signalCollision(Point ownPosition, Point ownDestination) {
    // pass provided coordinates to partner, partner should invoke it's collisionSpottedCallback()
}

void BLE_communicator::propose(int number_of_steps) {
    // pass given number to awaiting partner
}

int BLE_communicator::waitForProposal() {
    // block until partner invokes propose(), then return value he has sent
}

void registerGiveWayCallback() {
    // make sure that provided function will be called each time partner invokes his askToGiveWay()
}

void registerFreeWayCallback(void (Collision_avoidance::*callback)()) {
    // make sure that provided function will be called each time partner invokes his announceFreeWay()
}

void BLE_communicator::announceFreeWay() {
    // make sure partner invokes his freeWayAnnouncementCallback()
}

void BLE_communicator::respondToProposal(ResponseToProposal response) {
    // make sure partner will call his proposalResponseCallback()
}