/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: June 2023
*/

#include "wifi-config.h"
#define ENABLE_AVOCADO_ESP_WIFI_MONITOR
#define ENABLE_AVOCADO_ESP_WIFI_BOOT
#define DEVICE_NAME "MonsterCar" 
#include <AVOCADO_esp.h>
AVOCADO_esp esp;

#include "motor.h"

// debug
//#define DEBUG_SERIAL
#define DEBUG_WIFISERIAL
#define MOTOR_EN

// motor
int forward_vector = 0, left_vector = 0;
#define LEFT_VECTOR_K 0.2
// update max speed
#define MOTOR_SPEED_UPDATE_D 10
#define MOTOR_LEFT_SPEED_UPDATE_D 5
int max_motor_speed = 30;
int max_left_motor_speed = 20;

// servo
#include "servo_plus.h"
#include "my_servo.h"
ServoPlus servo_camera;
ServoPlus servo_left_arm;
ServoPlus servo_right_arm;
ServoPlus servo_tail;
#define SERVO_UPDATE_D 2
#define SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(a) (176-a)

#include "keyboard_config.h"

void setup(){
  Serial.begin(9600);
  MotorShield.setup();
  esp.setup();
  // servo config        pin, min_time, max_time, min_angle, max_angle
  servo_camera.attach   (D2,  350,      2600,     58,        180);
  servo_left_arm.attach (D4,  350,      2600,     0,         180);
  servo_right_arm.attach(D3,  350,      2600,     0,         180);
  servo_tail.attach     (D1,  500,      2500,     0,         180);
  // start servo
  servo_camera.write(94);
  servo_left_arm.write(0); // SERVO_MIN_LEFT_ANGLE
  servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.read()));
  servo_tail.write(170);
}

void loop(){
  robot_main();
}

void robot_main() {
  esp.update();
  if (esp.available()) {
    String input_data = esp.read();
    //esp.print(input_data);
    servo_input(input_data);
    motor_input(input_data);
    ladder(input_data);
  }
}

void ladder(String input_data) {
  #ifdef MOTOR_EN
  const int left_arm_up = 38;
  const int left_arm_down = 172;
  const int tail_up = 170;
  const int tail_down = 10;
  const int servo_speed_dt = 10;
  if (input_data==LADDER_BUTTON) { //  || input_data==LADDER_XN_BUTTON
    for (int i = 0; i<1; i++) {
      // вернуться в состояние по умолчанию
      MotorShield.motors(0,0);
      servo_camera.write(94);
      servo_left_arm.write(left_arm_up);
      servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.read()));
      servo_tail.write(tail_up);
      delay(2000);
      // "врезаться" в ступеньку
      MotorShield.motors(10,10);
      delay(2000);
      // передними лапками поднять первые ведущие колеса на ступеньку
      MotorShield.motors(20,20);
      for (int i = left_arm_up; i<left_arm_down; i++) {
        servo_left_arm.write(i);
        servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.read()));  
        delay(servo_speed_dt);
      }
      delay(1000);
      // поднять обратно передние лапки
      MotorShield.motors(0,0);
      for (int i = left_arm_down; i>left_arm_up; i--) {
        servo_left_arm.write(i);
        servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.read()));  
        delay(servo_speed_dt);
      }
      // опустить хвост и поднять зад, тем самым робот заедет на ступеньки
      //MotorShield.motors(20,20);
      for (int i = tail_up; i>tail_down; i--) {
        servo_tail.write(i);
        delay(servo_speed_dt);
      }
      delay(1000);
      //
      //MotorShield.motors(0,0);
      //delay(1000); 
      //
      MotorShield.motors(60,60);
      delay(1000);
      // мы по идее заехали, так что опускаем хвост
      MotorShield.motors(0,0);
      for (int i = tail_down; i<tail_up; i++) {
        servo_tail.write(i);
        delay(servo_speed_dt);
      }
      
      
      // the end
      MotorShield.motors(0,0);
      servo_camera.write(94);
      servo_left_arm.write(38);
      servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.read()));
      servo_tail.write(170);
    }
  }
  #else
  print_data("WARNING: motor disable!");
  #endif
}

void servo_input(String input_data) {
  // camera
  if (input_data==SERVO_CAMERA_UP_BUTTON || input_data==SERVO_CAMERA_DOWN_BUTTON) {
    if (input_data==SERVO_CAMERA_UP_BUTTON) servo_camera.write_on(-SERVO_UPDATE_D);
    else servo_camera.write_on(SERVO_UPDATE_D);
    print_data("camera_angle: " + String(servo_camera.read()));
  }
  // arm
  if (input_data==SERVO_ARM_UP_BUTTON || input_data==SERVO_ARM_DOWN_BUTTON) {
    if (input_data==SERVO_ARM_UP_BUTTON) servo_left_arm.write_on(-SERVO_UPDATE_D);
    else  servo_left_arm.write_on(SERVO_UPDATE_D);
    servo_right_arm.write(SERVO_TRANSLATE_LEFT_TO_RIGHT_ANGLE(servo_left_arm.read()));
    print_data("arm_angle: " + String(servo_left_arm.read()) + " " + String(servo_right_arm.read()));
  }
  // tail
  if (input_data==SERVO_TAIL_UP_BUTTON || input_data==SERVO_TAIL_DOWN_BUTTON) {
    if (input_data==SERVO_TAIL_UP_BUTTON) servo_tail.write(servo_tail.read()+SERVO_UPDATE_D);
    else  servo_tail.write(servo_tail.read()-SERVO_UPDATE_D);
    print_data("tail_angle: " + String(servo_tail.read()));
  }
}

void motor_input(String input_data) {
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
  MotorShield.motors(left_speed*0.8,right_speed);
  #else
  if (left_speed!=0 || right_speed!=0) print_data("WARNING: motor disable!");
  #endif
}

void print_data(String a) {
  #ifdef DEBUG_SERIAL
  Serial.println(a);
  #endif
  #ifdef DEBUG_WIFISERIAL
  esp.print(a);
  #endif
}
