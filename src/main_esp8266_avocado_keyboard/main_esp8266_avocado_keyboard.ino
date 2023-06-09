/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: June 2023
*/

#include "wifi-config.h"
//#define WIFI_NAME "test_name"
//#define WIFI_PASSWORD "test_pass"
//#define SERVER_IP "192.168.1.82"
//#define SERVER_PORT 1234
#define ENABLE_AVOCADO_ESP_WIFI_MONITOR
#define ENABLE_AVOCADO_ESP_WIFI_BOOT
#define DEVICE_NAME "MonsterCar" 
#include <AVOCADO_esp.h>
AVOCADO_esp esp;

#include "motor.h"

// debug
//#define DEBUG_SERIAL
#define DEBUG_WIFISERIAL
//#define MOTOR_EN

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
#include "my_servo.h"
MyServo servo_camera;
#define SERVO_CAMERA_UP_BUTTON "+38"
#define SERVO_CAMERA_DOWN_BUTTON "+40"
#define SERVO_CAMERA_UPDATE_D 2
#define SERVO_CAMERA_PIN D2
#define SERVO_CAMERA_MAX_ANGLE 270
#define SERVO_CAMERA_MIN_ANGLE 86

#define SERVO_UPDATE_D 2

MyServo servo_left_arm;
MyServo servo_right_arm;
#define SERVO_LEFT_ARM_PIN D4
#define SERVO_RIGHT_ARM_PIN D3
#define SERVO_ARM_UP_BUTTON "+36"
#define SERVO_ARM_DOWN_BUTTON "+33"
#define SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(a) (275-a) // (265-a)
#define SERVO_MAX_LEFT_ANGLE 180
#define SERVO_MIN_LEFT_ANGLE 5

MyServo servo_tail;
#define SERVO_TAIL_PIN D1
#define SERVO_TAIL_UP_BUTTON "+34"
#define SERVO_TAIL_DOWN_BUTTON "+35"
#define SERVO_MAX_TAIL_ANGLE 180
#define SERVO_MIN_TAIL_ANGLE 0


//int servo_angle = 70;

void setup(){
  Serial.begin(9600);
  //pinMode(SERVO_CAMERA_PIN,OUTPUT);
  MotorShield.setup();
  esp.setup();
  servo_camera.attach(SERVO_CAMERA_PIN,350,2600,0,270);
  servo_left_arm.attach(SERVO_LEFT_ARM_PIN,350,2600,0,270);
  servo_right_arm.attach(SERVO_RIGHT_ARM_PIN,350,2600,0,270);
  servo_tail.attach(SERVO_TAIL_PIN,500,2500,0,180);
  servo_camera.write(150);
  servo_left_arm.write(SERVO_MIN_LEFT_ANGLE);
  servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.get_angle()));
  servo_tail.write(90);
}

void loop(){
  //test_gamepad();
  robot_main();
}

//void write_servo(int angle, int pin=SERVO_PIN) {
//  angle = constrain(angle, 0, 180);
//  int pos = map(angle, 0, 180, 544, 2400);
//  digitalWrite(pin, HIGH);
//  delayMicroseconds(pos);
//  digitalWrite(pin, LOW);
//  delayMicroseconds(20000 - pos);
//  delay(1);
//}

void robot_main() {
  esp.update();
  if (esp.available()) {
    String input_data = esp.read();
    esp.print(input_data);
    // servo camera
    if (input_data==SERVO_CAMERA_UP_BUTTON || input_data==SERVO_CAMERA_DOWN_BUTTON) {
      if (input_data==SERVO_CAMERA_UP_BUTTON) servo_camera.write_on(-SERVO_CAMERA_UPDATE_D);
      else servo_camera.write_on(SERVO_CAMERA_UPDATE_D);
      if (servo_camera.get_angle()<SERVO_CAMERA_MIN_ANGLE) servo_camera.write(SERVO_CAMERA_MIN_ANGLE);
      if (servo_camera.get_angle()>SERVO_CAMERA_MAX_ANGLE) servo_camera.write(SERVO_CAMERA_MAX_ANGLE);
      //servo_angle  = constrain(servo_angle,SERVO_MIN_ANGLE,SERVO_MAX_ANGLE);
      print_data("camera_angle: " + String(servo_camera.get_angle()));
    }
    // servo arm
    if (input_data==SERVO_ARM_UP_BUTTON || input_data==SERVO_ARM_DOWN_BUTTON) {
      if (input_data==SERVO_ARM_UP_BUTTON) servo_left_arm.write_on(-SERVO_UPDATE_D);
      else  servo_left_arm.write_on(SERVO_UPDATE_D);
      if (servo_camera.get_angle()<SERVO_MIN_LEFT_ANGLE) servo_camera.write(SERVO_MIN_LEFT_ANGLE);
      if (servo_camera.get_angle()>SERVO_MAX_LEFT_ANGLE) servo_camera.write(SERVO_MAX_LEFT_ANGLE);
      servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.get_angle()));
      print_data("arm_angle: " + String(servo_left_arm.get_angle()) + " " + String(servo_right_arm.get_angle()));
    }
    // servo tail
    if (input_data==SERVO_TAIL_UP_BUTTON || input_data==SERVO_TAIL_DOWN_BUTTON) {
      if (input_data==SERVO_ARM_UP_BUTTON) servo_left_arm.write_on(-SERVO_UPDATE_D);
      else  servo_left_arm.write_on(SERVO_UPDATE_D);
      if (servo_camera.get_angle()<SERVO_MIN_LEFT_ANGLE) servo_camera.write(SERVO_MIN_LEFT_ANGLE);
      if (servo_camera.get_angle()>SERVO_MAX_LEFT_ANGLE) servo_camera.write(SERVO_MAX_LEFT_ANGLE);
      servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.get_angle()));
      print_data("arm_angle: " + String(servo_left_arm.get_angle()) + " " + String(servo_right_arm.get_angle()));
    }
    // max_speed
    if (input_data==UP_SPEED_BUTTON || input_data==DOWN_SPEED_BUTTON) {
      if (input_data==UP_SPEED_BUTTON) max_motor_speed+=MOTOR_SPEED_UPDATE_D;
      else max_motor_speed-=MOTOR_SPEED_UPDATE_D;
      max_motor_speed = constrain(max_motor_speed,0,100);
      print_data("update max speed: "+String(max_motor_speed));
    }
    if (input_data==UP_LEFT_SPEED_BUTTON || input_data==DOWN_LEFT_SPEED_BUTTON) {
      if (input_data==UP_LEFT_SPEED_BUTTON) max_left_motor_speed+=MOTOR_LEFT_SPEED_UPDATE_D;
      else max_left_motor_speed-=MOTOR_LEFT_SPEED_UPDATE_D;
      max_left_motor_speed = constrain(max_left_motor_speed,0,100);
      print_data("update max left speed: "+String(max_left_motor_speed));
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
    else { 
      left_speed = forward_vector*max_motor_speed;
      right_speed = left_speed;
      left_speed = left_vector==1?left_speed*LEFT_VECTOR_K:left_speed;
      right_speed = left_vector==-1?right_speed*LEFT_VECTOR_K:right_speed;
    }
    #ifdef DEBUG_SERIAL
    Serial.println("motor_speed: " + String(left_speed) + " " + String(right_speed));
    #endif
    #ifdef MOTOR_EN
    MotorShield.motors(left_speed,right_speed);
    #else
    if (left_speed!=0 || right_speed!=0) print_data("WARNING: motor disable!");
    #endif
  }
  servo_camera.write();
  servo_left_arm.write();
  servo_right_arm.write();
  //write_servo(servo_angle);
}

void print_data(String a) {
  #ifdef DEBUG_SERIAL
  Serial.println(a);
  #endif
  #ifdef DEBUG_WIFISERIAL
  esp.print(a);
  #endif
}
