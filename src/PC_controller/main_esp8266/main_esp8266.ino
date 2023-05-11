/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: May 2023
*/

#include "gamepad.h"
#include "motor.h"

void setup(){
  Serial.begin(9600);
  MotorShield.setup();
//  gamepad.setup();
//  setup_motor();
}

void loop(){
//  gamepad.update();
//  motor(LEFT,  gamepad.joystick(PSS_LY));
//  motor(RIGHT, gamepad.joystick(PSS_RY));
//  gamepad.vibration = gamepad.button(PSB_BLUE);
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
