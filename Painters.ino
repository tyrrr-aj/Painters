#include "src/motors/motors.h"
#include "src/encoder/encoder.h"

#include "src/localization/localization.h"
#include "src/steering/Control.h"

void setup() {
  Serial.begin(115200);
  
//  RoutePlanner routePlanner;
  std::vector<Point*> path;//routePlanner.getPath("patterns/pattern1.txt");
  path.push_back(new Point(-1.0, 1.0));
  path.push_back(new Point(2, 2));
  path.push_back(new Point(3, 3));
 
  Encoder encoder;
  Motors motors;
  motors.addEncoder(&encoder);
  
  Localization localization(&encoder);
  Steering steering(&motors, &localization);
  Control control(path, steering);

  control.accomplishTrace();

}

void loop() {

}
  
