#ifndef BLE_COMMUNICATOR_H
#define BLE_COMMUNICATOR_H

#include <arduino.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "freertos/task.h"

#include "../geometry/Point.h"
#include "protocol.h"
#include "characteristics.h"

#define PAINTERS_SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define POINT_1_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define POINT_2_CHARACTERISTIC_UUID "beb1234e-36e1-4688-b7f5-ea07361b26a8"

class Collision_avoidance;

class BLE_communicator
{
    public:
		
		/*INITIALIZING METHODS*/
	
        BLE_communicator();
        BLE_communicator(Collision_avoidance* avoidance);
        void setUpCommunication();

		/*BLUETOOTH NOTIFYING METHODS*/
		
        void announceNewCourse(Point position, Point destination);
        void signalCollision(Point ownPosition, Point ownDestination);
        void propose(int number_of_steps);
        void respondToProposal(protocol::ResponseToProposal response);
		void announceFreeWay();
	
	private:
		Collision_avoidance* avoidance;
	
		NotificationCharacteristic* LOCAL_notificationCharacteristic;
		BLERemoteCharacteristic* REMOTE_notificationCharacteristic;
		
		CourseCharacteristic* LOCAL_courseCharacteristic;
		BLERemoteCharacteristic* REMOTE_courseCharacteristic;

		ProposalCharacteristic* LOCAL_proposalCharacteristic;
		BLERemoteCharacteristic* REMOTE_proposalCharacteristic;

		ResponseToProposalCharacteristic* LOCAL_responseCharacteristic;
		BLERemoteCharacteristic* REMOTE_responseCharacteristic;

		void notifyPartner(NotificationCode notificationCode);

		/*MESSAGES CALLBACK METHODS*/

		void newCourseCallback();
		void collisionSpottedCallback();
		void proposalCallback();
		void responseToProposalCallback();
		void freeWayCallback();

		/*BLUETOOTH CLIENT-SERVER METHODS*/
		
		void createServer();
		void scan();
		void connect();
		void setUpCharacteristics(BLEClient* client);
		void runListeningTask();
		
		void listen(void* parameter);

		/*BLUETOOTH EVENT-HANDLING METHODS*/
		
		void registerCallback(BLEClient* pClient);
		static void notificationCallback(BLERemoteCharacteristic* characteristic, uint8_t* pData, size_t length, bool isNotify);
		
		/*METHODS TO CODE AND ENCODE BLUETOOTH MESSAGES*/
		
		double hashTwoPoints(Point, Point);
		std::vector<Point> makeTwoPoints(long long);
};

#endif