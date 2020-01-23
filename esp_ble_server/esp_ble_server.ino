// COM 6

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUID_2 "beb2222e-36e1-4688-b7f5-ea07361b26a8"

struct MyData {
  double A;
  double B;
};

class MyDataCharacteristic : public BLECharacteristic {
  public:
    MyDataCharacteristic() : BLECharacteristic(BLEUUID(CHARACTERISTIC_UUID)) {}

    void setMyData(MyData data) {
      setValue((uint8_t*) &data, sizeof data);
    }

    MyData* getMyData() {
      return (MyData*) getData();
    }
};

class NotificationCharacteristic : public BLECharacteristic {
  public:
    NotificationCharacteristic() : BLECharacteristic(BLEUUID(CHARACTERISTIC_UUID_2)) {}

    void setNotificationCode(int code) {
      setValue(code);
    }
};

class MyDataCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onRead(BLECharacteristic* characteristic) {
    Serial.println("Value read by client!");
  }

  void onWrite(BLECharacteristic* characteristic) {
    
    Serial.print("Value changed! New value: A=");
    MyDataCharacteristic* ownCharacteristic = static_cast<MyDataCharacteristic*>(characteristic);
    MyData* newData = ownCharacteristic->getMyData();
    Serial.print(newData->A);
    Serial.print(", B=");
    Serial.println(newData->B);
  }
};

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) {
    Serial.println("Device connected");
  }

  void onDisconnect(BLEServer* server) {
    Serial.println("Device disconnected");
  }
};

NotificationCharacteristic* notificationCharacteristic;

void setupServer() {
  BLEDevice::init("SampleServer");
  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new MyServerCallbacks());
  BLEService* service = server->createService(SERVICE_UUID);
  
  MyDataCharacteristic* characteristic = new MyDataCharacteristic();
  characteristic->setReadProperty(true);
  characteristic->setWriteProperty(true);
  characteristic->setNotifyProperty(true);
  characteristic->setCallbacks(new MyDataCharacteristicCallbacks());
  service->addCharacteristic(characteristic);

  notificationCharacteristic = new NotificationCharacteristic();
  notificationCharacteristic->setNotifyProperty(true);
  service->addCharacteristic(notificationCharacteristic);
  
  MyData data;
  data.A = 0.0;
  data.B = 1.1;
  characteristic->setMyData(data);

  service->start();

  BLEAdvertising* advertising = server->getAdvertising();
  advertising->addServiceUUID(BLEUUID(SERVICE_UUID));
  advertising->start();

  Serial.println("Server started");
}

void setup() {
  Serial.begin(115200);
  setupServer();
}

int i = 0;
void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  notificationCharacteristic->setNotificationCode(i++);
  notificationCharacteristic->notify();
}
