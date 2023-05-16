/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: May 2023
*/

#include "wifi-config.h"
//#define WIFI_NAME "test_name"
//#define WIFI_PASSWORD "test_pass"
//#define SERVER_IP "192.168.1.82"
//#define SERVER_PORT 1234
#define ENABLE_AVOCADO_ESP_WIFI_GAMEPAD
#include <AVOCADO_esp.h>
AVOCADO_esp esp;

#include "motor.h"

void setup(){
  Serial.begin(9600);
  MotorShield.setup();
  esp.setup();
}

void loop(){
  //test_gamepad();
  esp.update();
  MotorShield.motors(-esp.gamepad_joystick[1],esp.gamepad_joystick[2]);
}
