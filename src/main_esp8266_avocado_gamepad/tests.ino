/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: May 2023
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
  
  // button
  for (int i = 0; i<GAMEPAD_BUTTON_QUANTITY; i++) {
    Serial.print(esp.gamepad_button[i]); // bool
  }

  Serial.print(" <> ");
  
  // allow
  for (int i = 0; i<GAMEPAD_ARROW_QUANTITY; i++) {
    Serial.print(esp.gamepad_arrow[i]); // int
    Serial.print(" ");
  }
  
  Serial.print(" <> ");
  
  // joystick
  for (int i = 0; i<GAMEPAD_JOYSTICK_QUANTITY; i++) {
    Serial.print(esp.gamepad_joystick[i]); // int
    Serial.print(" ");
  }
    
  Serial.println();
}
