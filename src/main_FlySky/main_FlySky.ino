/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  March 2024
   modify: March 2024
*/

#include "FlySky.h"
#include "motor.h"

#define EXACT_FORWARD_K 0.3
#define TURN_K 0.2

void setup(){
  // Serial.begin(115200);
  FlySky.begin(Serial); // Serial2
  MotorShield.setup();
}

void loop(){
  // for (int i = 0; i<14; i++) Serial.print(String(FlySky.readChannel(i)) + " "); Serial.println();
  int joystick_left_y = FlySky.readChannel(2);
  int joystick_left_x = FlySky.readChannel(3);
  int joystick_right_y = FlySky.readChannel(1);
  int joystick_right_x = FlySky.readChannel(0);
  int forward = joystick_right_y*EXACT_FORWARD_K + (joystick_left_y+100)/2;
  int turn = (joystick_left_x + joystick_right_x)*TURN_K;
  int left_speed = forward + turn;
  int right_speed = forward - turn;
  // Serial.println(String(left_speed) + " " + String(right_speed));
  MotorShield.motors(left_speed, right_speed);
  delay(10);
}

