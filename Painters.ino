#include "src/motors/motors.h"
#include "src/encoder/encoder.h"
#include "src/route_planning/RoutePlanner.h"
#include "src/Localization/localization.h"
#include "src/steering/Steering.h"

void setup() {
  RoutePlanner routePlanner;
  Motors motors;
  Encoder encoder;
  Localization localization(&encoder);
  
  motors.addEncoder(&encoder);
  
  std::vector<Point> path;
  Steering steering(&motors, &localization);
  
  path = routePlanner.getPath("patterns/pattern1.txt");
  for (int i = 0; i < path.size(); ++i)
  {
    steering.driveTo(path[i]);
  }
}

void loop() {
  
}
  
