#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static BLEUUID serviceUUID(SERVICE_UUID);

static BLEAdvertisedDevice* myDevice;

int scanTime = 3; //In seconds
BLEScan* pBLEScan;

BLECharacteristic *pCharacteristic;
BLERemoteCharacteristic* global_characteristic;

bool doConnect = false;
bool valueChanged = false;

void connect(BLEAdvertisedDevice* advertisedDevice);

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      //if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID))
      if (advertisedDevice.getName() == "PaintersServer")
      {
        BLEDevice::getScan()->stop();
        Serial.println("ESP found");
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        //connect(&advertisedDevice);
        doConnect = true;
      }
      else
      {
        Serial.println("this is not ESP32");   
      }
    }
};

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Device connected.");
  };
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Device disconnected.");
  }
};

void scan()
{
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value

  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  
}

void notifyCallback(BLERemoteCharacteristic* characteristic, uint8_t* pData, size_t length, bool isNotify) {
  valueChanged = true;
}

void registerCallback(BLEClient* pClient) {
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

void connect(BLEAdvertisedDevice* device)
{
  Serial.println("Connecting1...");
  //BLEAddress pAddress = device.getAddress();
  BLEClient*  pClient  = BLEDevice::createClient();
  pClient->connect(device);
  Serial.println("Connected to the server ");
  registerCallback(pClient);
}

void createServer()
{
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
int value;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println("[SERVER] Attempting to create server");
  createServer();
  Serial.println("[SERVER] Server created");
  
  //Serial.println("Scanning...");
  scan();
  Serial.println("Scan ended");
  if(doConnect)
  {
    Serial.println("[CLIENT] Attemting to connect to device");
    connect(myDevice);
    Serial.println("[CLIENT] Conncected to the server");
  }

  value=0;
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Inside loop");
  
  if(!doConnect){
 
     Serial.printf("*** NOTIFY: %d ***\n", pCharacteristic->getValue());
     pCharacteristic->setValue((uint8_t*)&value, 4); 
     pCharacteristic->notify(); 
     //pCharacteristic-&gt;indicate(); 
     ++value; 
  }
  else if(valueChanged){
     Serial.print("Read characteristic value: ");
     Serial.println(global_characteristic->readValue().c_str());
     valueChanged = false;
  }
  
  delay(500);
}
