#include "src/motors/motors.h"
#include "src/encoder/encoder.h"

Motors motors;
Encoder encoder;

void setup() {
  motors.addEncoder(&encoder);
  Serial.begin(115200);
  motors.leftMotor(100);
  motors.rightMotor(-100);
}

void loop() {
  Serial.print(encoder.getTicks(LEFT));
  Serial.print("\t");
  Serial.println(encoder.getTicks(RIGHT));
  delay(200);
}