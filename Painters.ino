#include "src/collision_avoidance/collision_avoidance.h"
#include "src/communication/ble_communicator.h"

Collision_avoidance* avoidance = new Collision_avoidance();
BLE_communicator* communicator = new BLE_communicator(avoidance);

  Serial.println("RoutePlanner created successfully");
  std::vector<Point*> path = routePlanner.getPath("/pattern3.txt");
  //path.push_back(new Point(20.0, 0.0));
  //path.push_back(new Point(0.0, 30.0));
  //path.push_back(new Point(-20.0, 0.0));

  for(int i = 0; i < path.size(); i++)
  {
    Serial.print(path[i]->X);
    Serial.print(" ");
    Serial.println(path[i]->Y);
  }
 
  Encoder encoder;
  PausableMotors motors;
  motors.addEncoder(&encoder);
  
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
  
