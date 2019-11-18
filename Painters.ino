#include "src/motors/motors.h"
#include "src/encoder/encoder.h"
#include "src/localization/localization.h"

Motors motors;
Encoder encoder;
Localization localization(&encoder);

void setup() {
  motors.addEncoder(&encoder);
  Serial.begin(115200);
  motors.leftMotor(100);
  motors.rightMotor(-100);
}

void loop() {
  Position position = localization.getCurrentPosition();
  Serial.print(position.X);
  Serial.print("\t");
  Serial.print(position.Y);
  Serial.print("\t");
  Serial.println(position.Rotation);
  delay(20);
}
