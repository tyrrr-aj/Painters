#ifndef BLE_COMMUNICATOR_H
#define BLE_COMMUNICATOR_H

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "../geometry/Point.h"
#include "../collision_avoidance/collision_avoidance.h"

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class BLE_communicator
{
    public:
		static BLEUUID serviceUUID(SERVICE_UUID);
		static BLEAdvertisedDevice* mySeverDevice;
	
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