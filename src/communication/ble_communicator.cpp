#include "ble_communicator.h"

BLE_communicator::BLE_communicator() {

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

/*********************************************************************
All methods below are private.
*********************************************************************/

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
  Serial.println("Connecting1...");
  //BLEAddress pAddress = device.getAddress();
  BLEClient*  pClient  = BLEDevice::createClient();
  pClient->connect(myServerDevice);
  Serial.println("Connected to the server ");
  registerCallback(pClient);
}

void BLE_communicator::registerCallback(BLEClient* pClient) {
  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  global_characteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (global_characteristic->canNotify()) {
    Serial.println("Characteristic suitable for notification");
    const uint8_t notifyOn[] = {0x1, 0x0};
    global_characteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notifyOn, 2, true);
    global_characteristic->registerForNotify(notifyCallback);
  }
  else {
    Serial.println("Remote characteristic unnotifiable");
  }
}

void BLE_communicator::notifyCallback(BLERemoteCharacteristic* characteristic, uint8_t* pData, size_t length, bool isNotify) {
  valueChanged = true;
}