#ifndef BLE_COMMUNICATOR_H
#define BLE_COMMUNICATOR_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include <string>

#include "../geometry/Point.h"
#include "../collision_avoidance/collision_avoidance.h"

const char* SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b\0";

enum ResponseToProposal {AGREE, REJECT};

typedef std::function<void(Point, Point)> newCourseCallback;
typedef std::function<void(Point, Point)> collisionSpottedCallback;
typedef std::function<void(ResponseToProposal)> responseToProposalCallback;
typedef std::function<void()> freeWayCallback;

class BLE_communicator
{
    public:
		static BLEUUID serviceUUID(SERVICE_UUID);
		static BLEAdvertisedDevice* mySeverDevice;
	
        BLE_communicator();
        void setUpCommunication();

        void registerNewCourseCallback(newCourseCallback callback);
        void registerCollisionSpottedCallback(collisionSpottedCallback callback);
        void registerResponseToProposalCallback(responseToProposalCallback callback);
        void registerFreeWayCallback(freeWayCallback callback);

        void announceNewCourse(Point position, Point destination);
        void signalCollision(Point ownPosition, Point ownDestination);
        void propose(int number_of_steps);
        int waitForProposal();
        void announceFreeWay();
        void respondToProposal(ResponseToProposal response);
	
	private:
		BLECharacteristic* pCharacteristic;
		BLERemoteCharacteristic* global_characteristic;
		
		bool doConnect = false;
		bool valueChanged = false;
		
		void createServer();
		void scan();
		void connect();
		
		void registerCallback(BLEClient* pClient);
		void notifyCallback(BLERemoteCharacteristic* characteristic, uint8_t* pData, size_t length, bool isNotify);
};

#endif