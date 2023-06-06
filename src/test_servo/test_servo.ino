#include "my_servo.h"

MyServo servo_1;

void setup() {
  servo_1.attach(A0);
  servo_1.write(90);
}

void loop() {
  servo_1.write();
}
