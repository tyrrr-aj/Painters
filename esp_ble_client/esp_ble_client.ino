// COM 4

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <BLEAddress.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID_2 "beb2222e-36e1-4688-b7f5-ea07361b26a8"

BLEClient* client;
bool found = false;
BLEAddress* serverAddress;

struct MyData {
  double A;
  double B;
};

class ScanningCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.println("device found");
    if (advertisedDevice.haveServiceUUID()) {
      Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
      BLEUUID serviceUUID = advertisedDevice.getServiceUUID();
      if (serviceUUID.equals(BLEUUID(SERVICE_UUID))) {
        Serial.println("Useful device found");
        serverAddress = new BLEAddress(advertisedDevice.getAddress());
        found = true;
        advertisedDevice.getScan()->stop();
      }
    }
  }
};

void notifyCallback(BLERemoteCharacteristic* characteristic, uint8_t* data, size_t length, bool isNotify) {
  Serial.print("Notification received! Code: ");
  Serial.println((int)*data);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Client started");
  
  BLEDevice::init("SampleClient");
  client = BLEDevice::createClient();
  BLEScan* scan = BLEDevice::getScan();
  scan->setAdvertisedDeviceCallbacks(new ScanningCallbacks());
  scan->start(5);
  Serial.println("Scan ended");
  
  if (found) {
    Serial.println("Trying to connect");
    client->connect(*serverAddress);
    Serial.println("Connected to server");
    BLERemoteService* remoteService = client->getService(SERVICE_UUID);
    BLERemoteCharacteristic* remoteChar = remoteService->getCharacteristic(CHARACTERISTIC_UUID);

    BLERemoteCharacteristic* remoteCharNotify = remoteService->getCharacteristic(CHARACTERISTIC_UUID_2);
    remoteCharNotify->registerForNotify(notifyCallback);

    remoteChar->readValue();
    Serial.println("trying to read raw data");
    uint8_t* rawData = remoteChar->readRawData();
    Serial.println("Raw data read");
    MyData* readData = reinterpret_cast<MyData*>(rawData);
    Serial.print("Read data: A=");
    Serial.print(readData->A);
    Serial.print(", B=");
    Serial.println(readData->B);

    delay(500);
    Serial.println("Setting new value: A=10.54, B=-13.01");
    MyData newData;
    newData.A = 10.54;
    newData.B = -13.01;
    remoteChar->writeValue((uint8_t*) &newData, sizeof newData);
  }
}

void loop() {
  delay(1000);
}
