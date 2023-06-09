/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: May 2023
*/

/*
// old constant

//#define SERVO_CAMERA_PIN D2
//#define SERVO_CAMERA_MAX_ANGLE 270
//#define SERVO_CAMERA_MIN_ANGLE 86
//#define SERVO_LEFT_ARM_PIN D4
//#define SERVO_RIGHT_ARM_PIN D3
//#define SERVO_MAX_LEFT_ANGLE 180
//#define SERVO_MIN_LEFT_ANGLE 5
//#define SERVO_TAIL_PIN D1
//#define SERVO_MAX_TAIL_ANGLE 180
//#define SERVO_MIN_TAIL_ANGLE 0
*/

void test_motor() {
  MotorShield.motor_left.run(100);
  for (int i = 0; i<100; i++) {
    MotorShield.motors(i,0);
    delay(5);
  }
  for (int i = 100; i>-100; i--) {
    MotorShield.motors(i,0);
    delay(5);
  }
  for (int i = -100; i<0; i++) {
    MotorShield.motors(i,0);
    delay(5);
  }
  MotorShield.motors(0,0);
  
  for (int i = 0; i<100; i++) {
    MotorShield.motors(0,i);
    delay(5);
  }
  for (int i = 100; i>-100; i--) {
    MotorShield.motors(0,i);
    delay(5);
  }
  for (int i = -100; i<0; i++) {
    MotorShield.motors(0,i);
    delay(5);
  }
  MotorShield.motors(0,0);
}

void test_gamepad() {
  esp.update();
  if (esp.available()) {
    String input_data = esp.read();
    Serial.println(input_data);
    esp.print("Hello!");
  }
}
