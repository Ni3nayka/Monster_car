// https://robot-kit.ru/DC-Motor-Driver-BTS7960/BTS7960/

#pragma once
#include <Arduino.h>

#define LEFT  1
#define RIGHT 2

#define MOTOR_PWM_A   5
#define MOTOR_DIR_A_1 4
#define MOTOR_DIR_A_2 3
#define MOTOR_PWM_B   6
#define MOTOR_DIR_B_1 7
#define MOTOR_DIR_B_2 8

void setup_motor() {
  pinMode(MOTOR_PWM_A,  OUTPUT);
  pinMode(MOTOR_PWM_B,  OUTPUT);
  pinMode(MOTOR_DIR_A_1,OUTPUT);
  pinMode(MOTOR_DIR_A_2,OUTPUT);
  pinMode(MOTOR_DIR_B_1,OUTPUT);
  pinMode(MOTOR_DIR_B_2,OUTPUT);
}

void motor(byte number, int speed) {
  speed = map(constrain(speed,-100,100),-100,100,-255,255);
  if (number==1) {
    analogWrite(MOTOR_PWM_A, abs(speed));
    digitalWrite(MOTOR_DIR_A_1,speed>0);
    digitalWrite(MOTOR_DIR_A_2,speed<0);
  }
  else if (number==2) {
    analogWrite(MOTOR_PWM_B, abs(speed));
    digitalWrite(MOTOR_DIR_B_1,speed>0);
    digitalWrite(MOTOR_DIR_B_2,speed<0);
  }
}
