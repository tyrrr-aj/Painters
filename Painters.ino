//#include "pins.h"
//#include "libraries/motors/motors.h"
#include "motors.h"

Motors motors;

void setup() {
  // put your setup code here, to run once:
  motors.init();
  motors.drive(255);
  delay(1000);
  motors.coast();
  delay(500);
  motors.drive(-128);
  delay(1000);
  motors.stop();
  delay(500);
  motors.leftMotor(200);
  motors.rightMotor(-200);
  delay(1000);
  motors.stop();
}

void loop() {
  // put your main code here, to run repeatedly:

}
