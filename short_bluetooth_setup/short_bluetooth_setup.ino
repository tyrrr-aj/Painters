#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "../src/control/Control.h"

void setup() {
  Serial.begin(115200);

  BLE_communicator communicator;
  communicator.setUpCommunication();
  
  //Serial.println("Scanning...");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("Inside loop");
  
  if(!doConnect){
 
     Serial.printf("*** NOTIFY: %d ***\n", pCharacteristic->getValue());
     pCharacteristic->setValue(value); 
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
