#include "ble_communicator.h"
#include "../collision_avoidance/collision_avoidance.h"

NotificationCode notificationCode;

BLEAdvertisedDevice* myServerDevice;
bool partnerFound = false;

/*********************** INITIALIZING METHODS ****************************/

BLE_communicator::BLE_communicator() {}

BLE_communicator::BLE_communicator(Collision_avoidance* avoidance) {
	this->avoidance = avoidance;
	notificationCode = NONE;
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
	LOCAL_courseCharacteristic->setCourse(position, destination);
	notifyPartner(NEW_COURSE);
}                                                 

void BLE_communicator::signalCollision(Point ownPosition, Point ownDestination) {
	LOCAL_courseCharacteristic->setCourse(ownPosition, ownDestination);
	notifyPartner(COLLISION_SPOTTED);
}

void BLE_communicator::propose(int number_of_steps) {
	LOCAL_proposalCharacteristic->setProposal(number_of_steps);
	notifyPartner(PROPOSAL);
}

void BLE_communicator::respondToProposal(protocol::ResponseToProposal response) {
	LOCAL_responseCharacteristic->setResponse(response);
	notifyPartner(RESPONSE_TO_PROPOSAL);
}

void BLE_communicator::announceFreeWay() {
    notifyPartner(FREE_WAY);
}

void BLE_communicator::notifyPartner(NotificationCode notificationCode) {
	LOCAL_notificationCharacteristic->setNotificationCode(notificationCode);
	LOCAL_notificationCharacteristic->notify();
}

void BLE_communicator::listen(void* selfRaw) {
	BLE_communicator* self = static_cast<BLE_communicator*>(selfRaw);
	while(true){
		switch (notificationCode) {
			case NEW_COURSE:
				self->newCourseCallback();
				break;

			case COLLISION_SPOTTED:
				self->collisionSpottedCallback();
				break;

			case PROPOSAL:
				self->proposalCallback();
				break;

			case RESPONSE_TO_PROPOSAL:
				self->responseToProposalCallback();
				break;

			case FREE_WAY:
				self->freeWayCallback();
				break;
		}
		notificationCode = NONE;
		delay(10);
	}
}

/*********************************************************************
All methods below are private.
*********************************************************************/

/***************************** BLUETOOTH MESSAGES CALLBACKS ******************************/

void BLE_communicator::newCourseCallback() {
	Course* newCourse = reinterpret_cast<Course*>(REMOTE_courseCharacteristic->readRawData());
	avoidance->reactToPartnersCourseChange(newCourse->position, newCourse->destination);
}

void BLE_communicator::collisionSpottedCallback() {
	Course* newCourse = reinterpret_cast<Course*>(REMOTE_courseCharacteristic->readRawData());
	avoidance->reactToCollisionSpottedMessage(newCourse->position, newCourse->destination);
}

void BLE_communicator::proposalCallback() {
	int* proposal = reinterpret_cast<int*>(REMOTE_proposalCharacteristic->readRawData());
	avoidance->reactToProposal(*proposal);
}

void BLE_communicator::responseToProposalCallback() {
	protocol::ResponseToProposal response = static_cast<protocol::ResponseToProposal>(*reinterpret_cast<int*>(REMOTE_responseCharacteristic->readRawData()));
	avoidance->reactToProposalResponse(response);
}

void BLE_communicator::freeWayCallback() {
	avoidance->reactToFreeWayAnnouncement();
}

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
  setUpCharacteristics(client);
  runListeningTask();
}

void BLE_communicator::runListeningTask() {
	xTaskCreatePinnedToCore(
		listen,
		"Listening task",
		10000,
		this,
		1,
		NULL,
		LISTENING_TASK_CORE
	);
}

/**************************** BLUETOOTH EVENT-HANDLING METHODS ****************************/


void BLE_communicator::setUpCharacteristics(BLEClient* client) {
  BLERemoteService* paintersRemoteService = client->getService(PAINTERS_SERVICE_UUID);
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