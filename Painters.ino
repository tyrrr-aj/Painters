#include "src/motors/motors.h"
#include "src/encoder/encoder.h"

Motors motors;
Encoder encoder;

void setup() {
  motors.addEncoder(&encoder);
  Serial.begin(115200);
  motors.drive(255);
}

void loop() {
  Serial.print(encoder.getTicks(LEFT));
  Serial.print("\t");
  Serial.println(encoder.getTicks(RIGHT));
  delay(200);
}
