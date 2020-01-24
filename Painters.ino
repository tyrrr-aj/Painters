#include "src/collision_avoidance/collision_avoidance.h"
#include "src/communication/ble_communicator.h"
#include "src/steering/steering.h"

Collision_avoidance* avoidance = new Collision_avoidance();
BLE_communicator* communicator = new BLE_communicator(avoidance);

void setup() {  
  avoidance->addCommunicator(communicator);

  communicator->setUpCommunication();
  delay(5000);
}

void loop() {
  Point source(47.4, 15.91);
  Point dest(1.4313, 97.499);
  
  communicator->announceNewCourse(source, dest);
  delay(5000);
}
  
