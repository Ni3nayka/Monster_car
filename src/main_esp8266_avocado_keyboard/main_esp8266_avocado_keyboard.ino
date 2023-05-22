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
#define ENABLE_AVOCADO_ESP_WIFI_MONITOR
#include <AVOCADO_esp.h>
AVOCADO_esp esp;

#include "motor.h"

// debug
//#define DEBUG_SERIAL
#define DEBUG_WIFISERIAL

// motor
#define MOTOR_FORWARD_BUTTON "+87"
#define MOTOR_BACKWARD_BUTTON "+83"
#define MOTOR_LEFT_BUTTON "+65"
#define MOTOR_RIGHT_BUTTON "+68"
#define MOTOR_FORWARD_BUTTON_OFF "-87"
#define MOTOR_BACKWARD_BUTTON_OFF "-83"
#define MOTOR_LEFT_BUTTON_OFF "-65"
#define MOTOR_RIGHT_BUTTON_OFF "-68"
int forward_vector = 0, left_vector = 0;
//#define FORWARD_VECTOR_K 90
//#define LEFT_VECTOR_K 100
#define LEFT_VECTOR_K 0.2

// update max speed
#define UP_SPEED_BUTTON "+69" //"+39"
#define DOWN_SPEED_BUTTON "+81" //"+37"
#define MOTOR_SPEED_UPDATE_D 10
#define UP_LEFT_SPEED_BUTTON "+67"
#define DOWN_LEFT_SPEED_BUTTON "+90"
#define MOTOR_LEFT_SPEED_UPDATE_D 5
int max_motor_speed = 30;
int max_left_motor_speed = 20;

// servo
#define SERVO_UP_BUTTON "+38"
#define SERVO_DOWN_BUTTON "+40"
#define SERVO_UPDATE_D 2
#define SERVO_PIN D2
#define SERVO_MAX_ANGLE 150
#define SERVO_MIN_ANGLE 20
int servo_angle = 70;

void setup(){
  Serial.begin(9600);
  pinMode(SERVO_PIN,OUTPUT);
  MotorShield.setup();
  esp.setup();
}

void loop(){
  //test_gamepad();
  robot_main();
}

void write_servo(int angle, int pin=SERVO_PIN) {
  angle = constrain(angle, 0, 180);
  int pos = map(angle, 0, 180, 544, 2400);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pos);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000 - pos);
  delay(1);
}

void robot_main() {
  esp.update();
  if (esp.available()) {
    String input_data = esp.read();
    // servo
    if (input_data==SERVO_UP_BUTTON || input_data==SERVO_DOWN_BUTTON) {
      if (input_data==SERVO_UP_BUTTON) servo_angle-=SERVO_UPDATE_D;
      else servo_angle+=SERVO_UPDATE_D;
      servo_angle = constrain(servo_angle,SERVO_MIN_ANGLE,SERVO_MAX_ANGLE);
      #ifdef DEBUG_SERIAL
      Serial.println("servo_angle: " + String(servo_angle));
      #endif
      #ifdef DEBUG_WIFISERIAL
      esp.print("servo_angle: " + String(servo_angle));
      #endif
    }
    // max_speed
    if (input_data==UP_SPEED_BUTTON || input_data==DOWN_SPEED_BUTTON) {
      if (input_data==UP_SPEED_BUTTON) max_motor_speed+=MOTOR_SPEED_UPDATE_D;
      else max_motor_speed-=MOTOR_SPEED_UPDATE_D;
      max_motor_speed = constrain(max_motor_speed,0,100);
      #ifdef DEBUG_SERIAL
      Serial.println("update max speed: "+String(max_motor_speed));
      #endif
      #ifdef DEBUG_WIFISERIAL
      esp.print("update max speed: "+String(max_motor_speed));
      #endif
    }
    if (input_data==UP_LEFT_SPEED_BUTTON || input_data==DOWN_LEFT_SPEED_BUTTON) {
      if (input_data==UP_LEFT_SPEED_BUTTON) max_left_motor_speed+=MOTOR_LEFT_SPEED_UPDATE_D;
      else max_left_motor_speed-=MOTOR_LEFT_SPEED_UPDATE_D;
      max_left_motor_speed = constrain(max_left_motor_speed,0,100);
      #ifdef DEBUG_SERIAL
      Serial.println("update max left speed: "+String(max_left_motor_speed));
      #endif
      #ifdef DEBUG_WIFISERIAL
      esp.print("update max left speed: "+String(max_left_motor_speed));
      #endif
    }
    // motors
    if (input_data==MOTOR_FORWARD_BUTTON) forward_vector = 1;
    if (input_data==MOTOR_BACKWARD_BUTTON) forward_vector = -1;
    if (input_data==MOTOR_FORWARD_BUTTON_OFF || input_data==MOTOR_BACKWARD_BUTTON_OFF) forward_vector = 0;
    if (input_data==MOTOR_LEFT_BUTTON) left_vector = 1;
    if (input_data==MOTOR_RIGHT_BUTTON) left_vector = -1;
    if (input_data==MOTOR_LEFT_BUTTON_OFF || input_data==MOTOR_RIGHT_BUTTON_OFF) left_vector = 0;
    // run motors
    int left_speed = 0;
    int right_speed = 0;
    if (forward_vector==0) {
      right_speed = left_vector*max_left_motor_speed;
      left_speed = -right_speed;
    }
    else { // if (left_vector==0) {
      left_speed = forward_vector*max_motor_speed;
      right_speed = left_speed;
      left_speed = left_vector==1?left_speed*LEFT_VECTOR_K:left_speed;
      right_speed = left_vector==-1?right_speed*LEFT_VECTOR_K:right_speed;
    }
//    else {
//      left_speed = max_left_motor_speed==1?forward_vector*max_motor_speed*0.5
//      right_speed =
////      left_speed = constrain(forward_vector*FORWARD_VECTOR_K-left_vector*LEFT_VECTOR_K,-max_motor_speed,max_motor_speed);
////      right_speed = constrain(forward_vector*FORWARD_VECTOR_K+left_vector*LEFT_VECTOR_K,-max_motor_speed,max_motor_speed);  
//    }
    #ifdef DEBUG_SERIAL
    Serial.println("motor_speed: " + String(left_speed) + " " + String(right_speed));
    #endif
    MotorShield.motors(left_speed,right_speed);
  }
  write_servo(servo_angle);
}
