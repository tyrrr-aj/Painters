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

#include "../geometry/Point.h"
//#include "../collision_avoidance/collision_avoidance.h"
#include "protocol.h"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define POINT_1_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define POINT_2_CHARACTERISTIC_UUID "beb1234e-36e1-4688-b7f5-ea07361b26a8"

class Collision_avoidance;

class MyPointCharacteristic : public BLECharacteristic {
	public:
	MyPointCharacteristic(const char* uuid) : BLECharacteristic(BLEUUID(uuid)){
		Point point(0,0);
		setPoint(point);
	}

	Point getPoint()
	{
		std::string rawData = getValue();
		std::stringstream s;
		Point* point = new Point();
		s.str(rawData);
		s >> point->X >> point->Y;
		return *point;
	}

	void setPoint(Point point){
		std::string text;
		std::stringstream s;
		s.str(text);
		s << point.X << " " << point.Y;
		setValue(s.str());
	}
};

class MyPointRemoteCharacteristic : public BLERemoteCharacteristic {	

	Point getPoint()
	{
		std::string rawData = readValue();
		std::stringstream s;
		Point* point = new Point();
		s.str(rawData);
		s >> point->X >> point->Y;
		return *point;
	}

	void setPoint(Point point)
	{
		std::string text;
		std::stringstream s;
		s.str(text);
		s << point.X << " " << point.Y;
		writeValue(s.str());
	}
};

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
		
		void listen();
	
	private:
		Collision_avoidance* avoidance;
	
		MyPointCharacteristic* p1Characteristic;
		MyPointCharacteristic* p2Characteristic;
		BLERemoteCharacteristic* global_characteristic;
		
		/*BLUETOOTH CLIENT-SERVER METHODS*/
		
		void createServer();
		void scan();
		void connect();
		
		/*BLUETOOTH EVENT-HANDLING METHODS*/
		
		void registerCallback(BLEClient* pClient);
		static void notifyCallback(BLERemoteCharacteristic* characteristic, uint8_t* pData, size_t length, bool isNotify);
		
		/*METHODS TO CODE AND ENCODE BLUETOOTH MESSAGES*/
		
		double hashTwoPoints(Point, Point);
		std::vector<Point> makeTwoPoints(long long);
};

#endif