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


// aceleration
#define SPEED_A 1
#define sgn(a) (a==0?0:(a>0?1:-1))
#define speed_up(old_speed,new_speed) (abs(old_speed)<abs(new_speed) && sgn(old_speed)==sgn(new_speed))
int old_left_speed = 0, old_right_speed = 0;
int old_forward_vector = 0, old_left_vector = 0;

// update max speed
#define UPDATE_MAX_SPEED_D 10
#define UP_SPEED_BUTTON_1 4
#define UP_SPEED_BUTTON_2 5
#define DOWN_SPEED_BUTTON_1 6
#define DOWN_SPEED_BUTTON_2 7
bool up_speed_1 = 0, up_speed_2 = 0, down_speed_1 = 0, down_speed_2 = 0;
int max_motor_speed = 50;

// servo
#define UPDATE_SERVO_ARROW 1
int servo_angle = 90;

void setup(){
  Serial.begin(9600);
  MotorShield.setup();
  esp.setup();
}

void loop(){
  //test_gamepad();
  robot_main();
}

void robot_main() {
  esp.update();
  // arrows: servo
  if (esp.gamepad_arrow[UPDATE_SERVO_ARROW]!=0) {
    servo_angle += esp.gamepad_arrow[UPDATE_SERVO_ARROW];
    servo_angle = constrain(servo_angle,0,180);
  }
  // button: max_speed
  if (esp.gamepad_button[UP_SPEED_BUTTON_1]==1 && up_speed_1==0 || esp.gamepad_button[UP_SPEED_BUTTON_2]==1 && up_speed_2==0) {
    max_motor_speed += UPDATE_MAX_SPEED_D;
    max_motor_speed = constrain(max_motor_speed,0,100);
    Serial.println("update max speed: "+String(max_motor_speed));
  }
  if (esp.gamepad_button[DOWN_SPEED_BUTTON_1]==1 && down_speed_1==0 || esp.gamepad_button[DOWN_SPEED_BUTTON_2]==1 && down_speed_2==0) {
    max_motor_speed -= UPDATE_MAX_SPEED_D;
    max_motor_speed = constrain(max_motor_speed,0,100);
    Serial.println("update max speed: "+String(max_motor_speed));
  }
  up_speed_1 = esp.gamepad_button[UP_SPEED_BUTTON_1];
  up_speed_2 = esp.gamepad_button[UP_SPEED_BUTTON_2];
  down_speed_1 = esp.gamepad_button[DOWN_SPEED_BUTTON_1];
  down_speed_2 = esp.gamepad_button[DOWN_SPEED_BUTTON_2];
  // joystick: motors
  int forward_vector = esp.gamepad_joystick[1];
  int left_vector = esp.gamepad_joystick[3];
//  if (speed_up(old_forward_vector,forward_vector)) {
//    int d = old_forward_vector - forward_vector;
//    d = d>SPEED_A?SPEED_A:(d<-SPEED_A?-SPEED_A:d);
//    forward_vector = old_forward_vector + d;
//  }
//  old_forward_vector = forward_vector;
//  old_left_vector = left_vector;
  int left_speed = constrain(forward_vector-left_vector,-max_motor_speed,max_motor_speed);
  int right_speed = constrain(forward_vector+left_vector,-max_motor_speed,max_motor_speed);
  MotorShield.motors(left_speed,right_speed);
  
}
