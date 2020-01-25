#include "src/motors/pausable_motors.h"
#include "src/encoder/encoder.h"
#include "src/route_planning/RoutePlanner.h"
#include "src/localization/localization.h"
#include "src/control/Control.h"
#include "src/file_loader/file_loader.h"
#include "src/geometry/Point.h"

void setup() {
  Serial.begin(115200); // for debug purposes

  FileLoader fileLoader;
  RoutePlanner routePlanner(&fileLoader);

  Serial.println("RoutePlanner created successfully");
  std::vector<Point*> path = routePlanner.getPath("/pattern1.txt");
 
  Encoder encoder;
  PausableMotors motors;
  motors.addEncoder(&encoder);
  
  Localization localization(&encoder);
  Steering steering(&motors, &localization);
  //BLE_communicator communicator;
  //Collision_avoidance collision_avoidance(&communicator, &steering, &localization);
  Control control(path, &steering, NULL);

  control.accomplishTrace();

}

void loop() {

}
