#include "ble_communicator.h"
#include "../collision_avoidance/collision_avoidance.h"

bool valueChanged = false;

BLEAdvertisedDevice* myServerDevice;
bool doConnect = false;

/*********************** INITIALIZING METHODS ****************************/

BLE_communicator::BLE_communicator() {

}

BLE_communicator::BLE_communicator(Collision_avoidance* avoidance) {
	this->avoidance = avoidance;
	valueChanged = false;
}

void BLE_communicator::setUpCommunication() {
    // create servers and clients, establish connections
	
	Serial.println("[SERVER] Attempting to create server");
	createServer();
	Serial.println("[SERVER] Server created");
	
	scan();
	Serial.println("Scan ended");
	if(doConnect)
	{
		Serial.println("[CLIENT] Attemting to connect to device");
		connect();
		Serial.println("[CLIENT] Conncected to the server");
	}

}

/*********************** BLUETOOTH NOTIFYING METHODS ****************************/


void BLE_communicator::announceNewCourse(Point position, Point destination) {
    // pass provided coordinates to partner, partner should invoke it's newCourseCalback()
	
    Serial.println("*********NEW COURSE SENDING********");
	Serial.println("");
	int notifyType = 51;
	double value = hashTwoPoints(position, destination);
	value += (double) notifyType;

	Serial.print("Sending value to client10:  ");
    Serial.println(value);

	value = 1539.0;
	int val = 10;
	pCharacteristic->setValue(val);
	pCharacteristic->notify();
	Serial.println("");
    Serial.println("*****************************");
}                                                    

void BLE_communicator::signalCollision(Point ownPosition, Point ownDestination) {
    // pass provided coordinates to partner, partner should invoke it's collisionSpottedCallback()
	
	int notifyType = 52;
	double value = hashTwoPoints(ownPosition, ownDestination);
	value += (double) notifyType;
	
	pCharacteristic->setValue(value);
	pCharacteristic->notify();
}

void BLE_communicator::propose(int number_of_steps) {
    // pass given number to awaiting partner
	
	int notifyType = 53;
	double value = (double) (100 * number_of_steps);
	value += (double) notifyType;
	
	pCharacteristic->setValue(value);
	pCharacteristic->notify();
}

void BLE_communicator::respondToProposal(protocol::ResponseToProposal response) {
    // make sure partner will call his proposalResponseCallback()
	
	//TODO
}

void BLE_communicator::announceFreeWay() {
    // make sure partner invokes his freeWayAnnouncementCallback()
	
	int notifyType = 55;
	double value = (double) notifyType;
	
	pCharacteristic->setValue(value);
	pCharacteristic->notify();
}

void BLE_communicator::listen() {
	while(true){
		if(valueChanged){
			const char* receivedString = global_characteristic->readValue().c_str();
            char* p;
			double receivedData = std::strtod(receivedString, &p);
			long long message = (long long) receivedData;
			
			int notifyType = message % 100;
			
			switch(notifyType){
				case 51: {
					std::vector<Point> course = makeTwoPoints(message);
					Point position = course[0];
					Point destination = course[1];
					
                    Serial.println("*********NEW COURSE received***********");
					Serial.println("Received new partner's course:");
					Serial.print("(X,Y) = (");
					Serial.print(position.X);
					Serial.print(", ");
					Serial.print(position.Y);
					Serial.println(")");
					
					Serial.print("(X,Y) = (");
					Serial.print(destination.X);
					Serial.print(", ");
					Serial.print(destination.Y);
					Serial.println(")");
					Serial.println("****************");

					//avoidance->reactToPartnersCourseChange(position, destination);
				}
				break;
				case 52: {
					std::vector<Point> course = makeTwoPoints(message);
					Point position = course[0];
					Point destination = course[1];
					
					avoidance->reactToCollisionSpottedMessage(position, destination);
				}	
				break;
				case 53: {
					int number_of_steps = (int) (message / 100);
					
					avoidance->reactToProposal(number_of_steps);
				}	
				break;
				case 54: {
				
				}	
				break;
				case 55: {
					avoidance->reactToFreeWayAnnouncement();
				}	
				break;
			}
			valueChanged = false;
		}
	}
}

/*********************************************************************
All methods below are private.
*********************************************************************/


/**************************** BLUETOOTH CLIENT-SERVER METHODS ****************************/


class MyServerCallbacks : public BLEServerCallbacks {
	void onConnect(BLEServer* pServer) {
		Serial.println("Device connected.");
	};
	void onDisconnect(BLEServer* pServer) {
		Serial.println("Device disconnected.");
	}
};

void BLE_communicator::createServer(){
	//Create the BLE Device
	BLEDevice::init("PaintersServer");
	
	//Create the BLE Server
	BLEServer *pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());
	
	//Create the BLE Service
	BLEService *pService = pServer->createService(SERVICE_UUID);
	
	//Create a BLE Characteristic
	pCharacteristic = pService->createCharacteristic(
										CHARACTERISTIC_UUID,
										BLECharacteristic::PROPERTY_READ |
										BLECharacteristic::PROPERTY_WRITE |
										BLECharacteristic::PROPERTY_NOTIFY |
										BLECharacteristic::PROPERTY_INDICATE
										);
	
	pCharacteristic->addDescriptor(new BLE2902());
	pCharacteristic->setValue("Hello World says Neil");
	pService->start();
	
	// BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
	pAdvertising->setMinPreferred(0x12);
	BLEDevice::startAdvertising();
	Serial.println("PaintersServer created");
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      //if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
      if (advertisedDevice.getName() == "PaintersServer")
      {
        BLEDevice::getScan()->stop();
        Serial.println("ESP found");
        myServerDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
      }
      else
      {
        Serial.println("Not ESP32");   
      }
    }
};

void BLE_communicator::scan()
{
	Serial.println("Scanning...");
	int scanTime = 3;
	
	//Create the BLE Device
	BLEDevice::init("");
	
	//Create the BLE Scan
	BLEScan* pBLEScan = BLEDevice::getScan();
	pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
	pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
	pBLEScan->setInterval(100);
	pBLEScan->setWindow(99);  // less or equal setInterval value
	
	BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
	pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  
}

void BLE_communicator::connect()
{
  Serial.println("Connecting...");
  //BLEAddress pAddress = device.getAddress();
  BLEClient*  pClient  = BLEDevice::createClient();
  pClient->connect(myServerDevice);
  Serial.println("Connected to the server ");
  registerCallback(pClient);
}


/**************************** BLUETOOTH EVENT-HANDLING METHODS ****************************/


void BLE_communicator::registerCallback(BLEClient* pClient) {
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  global_characteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (global_characteristic->canNotify()) {
    Serial.println("Characteristic suitable for notification");
    const uint8_t notifyOn[] = {0x1, 0x0};
    global_characteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notifyOn, 2, true);
    global_characteristic->registerForNotify(notifyCallback);
    Serial.println("Notify callback registererd for notifications");
  }
  else {
    Serial.println("Remote characteristic unnotifiable");
  }
}

void BLE_communicator::notifyCallback(BLERemoteCharacteristic* characteristic, uint8_t* pData, size_t length, bool isNotify) {
  valueChanged = true;
}


/**************************** METHODS TO CODE AND ENCODE BLUETOOTH MESSAGE ****************************/


double BLE_communicator::hashTwoPoints(Point source, Point dest) {
	std::vector<int> coords;
	coords.push_back( (int) std::floor(source.X * 10 + 0.5) );
	coords.push_back( (int) std::floor(source.Y * 10 + 0.5) );
	coords.push_back( (int) std::floor(dest.X * 10 + 0.5) );
	coords.push_back( 0/*(int) std::floor(dest.Y * 10 + 0.5) */);
	
	long factor = 100;
	long message = 0;
	for (int i = 0; i < coords.size() - 1; ++i)
	{
		message += factor * coords[i];
		factor *= 1000;
	}
	
	Serial.print("Message:	");
	Serial.println(message);
	
	return (double) message;
}

std::vector<Point> BLE_communicator::makeTwoPoints(long long value)
{
	long long message = (value / 100);
	double srcX = (message % 1000) / 10; message /= 1000;
	double srcY = (message % 1000) / 10; message /= 1000;
	Point source(srcX, srcY);

	double dstX = (message % 1000) / 10; message /= 1000;
	double dstY = (message % 1000) / 10; message /= 1000;
	Point dest(dstX, dstY);

	std::vector<Point> pointsVector;
	pointsVector.push_back(source);
	pointsVector.push_back(dest);
	return pointsVector;
}