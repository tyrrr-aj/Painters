#include "ble_communicator.h"
#include "../collision_avoidance/collision_avoidance.h"

NotificationCode NotificationCode = NONE;

BLEAdvertisedDevice* myServerDevice;
bool partnerFound = false;

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
	if(partnerFound)
	{
		Serial.println("[CLIENT] Attemting to connect to device");
		connect();
		Serial.println("[CLIENT] Conncected to the server");
	}

}

/*********************** BLUETOOTH NOTIFYING METHODS ****************************/


void BLE_communicator::announceNewCourse(Point position, Point destination) {
    // pass provided coordinates to partner, partner should invoke it's newCourseCalback()
	
	
}                                                 

void BLE_communicator::signalCollision(Point ownPosition, Point ownDestination) {
    // pass provided coordinates to partner, partner should invoke it's collisionSpottedCallback()
	
}

void BLE_communicator::propose(int number_of_steps) {
    // pass given number to awaiting partner
	
}

void BLE_communicator::respondToProposal(protocol::ResponseToProposal response) {
    // make sure partner will call his proposalResponseCallback()
	
}

void BLE_communicator::announceFreeWay() {
    // make sure partner invokes his freeWayAnnouncementCallback()
	
}

void BLE_communicator::listen() {
	while(true){
		if(valueChanged){
			
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
	BLEServer *server = BLEDevice::createServer();
	server->setCallbacks(new MyServerCallbacks());
	
	//Create the BLE Service
	BLEService *paintersService = server->createService(PAINTERS_SERVICE_UUID);
	
	//Create a BLE Characteristic

	LOCAL_notificationCharacteristic = new NotificationCharacteristic();
	LOCAL_courseCharacteristic = new CourseCharacteristic();
	LOCAL_proposalCharacteristic = new ProposalCharacteristic();
	LOCAL_responseCharacteristic = new ResponseToProposalCharacteristic();

	paintersService->addCharacteristic(LOCAL_notificationCharacteristic);
	paintersService->addCharacteristic(LOCAL_courseCharacteristic);
	paintersService->addCharacteristic(LOCAL_proposalCharacteristic);
	paintersService->addCharacteristic(LOCAL_responseCharacteristic);

	paintersService->start();
	
	// BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(PAINTERS_SERVICE_UUID);
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
        partnerFound = true;
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
  BLEClient* client  = BLEDevice::createClient();
  client->connect(myServerDevice);
  Serial.println("Connected to the server ");
  setUpCharacterisitcs(client);
}


/**************************** BLUETOOTH EVENT-HANDLING METHODS ****************************/


void BLE_communicator::setUpCharacteristics(BLEClient* client) {
  BLERemoteService* paintersRemoteService = pClient->getService(PAINTERS_SERVICE_UUID);
  REMOTE_notificationCharacteristic = paintersRemoteService->getCharacteristic(NOTIFICATION_CHARACTERISTIC_UUID);
  REMOTE_courseCharacteristic = paintersRemoteService->getCharacteristic(COURSE_CHARACTERISTIC_UUID);
  REMOTE_proposalCharacteristic = paintersRemoteService->getCharacteristic(PROPOSAL_CHARACTERISTIC_UUID);
  REMOTE_responseCharacteristic = paintersRemoteService->getCharacteristic(RESPONSE_TO_PROPOSAL_CHARACTERISTIC_UUID);

  if (REMOTE_notificationCharacteristic->canNotify()) {
    Serial.println("Characteristic suitable for notification");
    // const uint8_t notifyOn[] = {0x1, 0x0};
    // global_characteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notifyOn, 2, true);
    REMOTE_notificationCharacteristic->registerForNotify(notificationCallback);
  }
  else {
    Serial.println("Remote characteristic unnotifiable");
  }
}

void BLE_communicator::notificationCallback(BLERemoteCharacteristic* characteristic, uint8_t* pData, size_t length, bool isNotify) {
  notificationCode = (NotificationCode) *pData;
}