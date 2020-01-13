#include "src/motors/motors.h"
#include "src/encoder/encoder.h"
#include "src/route_planning/RoutePlanner.h"
#include "src/localization/localization.h"
#include "src/control/Control.h"
#include "src/file_loader/file_loader.h"
#include "src/geometry/Point.h"

void setup() {
  Serial.begin(115200);

  FileLoader fileLoader;
  RoutePlanner routePlanner(&fileLoader);

  Serial.println("RoutePlanner created successfully");
  std::vector<Point*> path;// = routePlanner.getPath("/pattern1.txt");
  path.push_back(new Point(100.0, 0.0));
  path.push_back(new Point(0.0, 0.0));
  
  for(int i = 0; i < path.size(); i++) {
    Serial.print(path[i]->X);
    Serial.print(" ");
    Serial.println(path[i]->Y);
  }
 
  Encoder encoder;
  PausableMotors motors;
  motors.addEncoder(&encoder);
  
  Localization localization(&encoder);
  Steering steering(&motors, &localization);
  //BLE_communicator communicator;
  Collision_avoidance collision_avoidance;//(&communicator, &steering, &localization);
  Control control(path, &steering, &collision_avoidance);

  control.accomplishTrace();

}

void loop() {

}
  
