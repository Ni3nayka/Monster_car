/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: Match 2024
*/

#pragma once

#define MOTOR_DIR_A_1 3
#define MOTOR_DIR_A_2 5
#define MOTOR_DIR_B_1 6
#define MOTOR_DIR_B_2 9

class Motor {
  public:
    //int reverse;
    void setup(int pin1, int pin2) {
      Motor::pin1 = pin1;
      Motor::pin2 = pin2;
      pinMode(Motor::pin1,OUTPUT);
      pinMode(Motor::pin2,OUTPUT);
      //reverse = 1;
    }
    void run(int speed) {
      speed = map(constrain(speed,-100,100),-100,100,-255,255);
      //speed *= Motor::reverse;
      analogWrite(Motor::pin1, speed>0?speed:0);
      analogWrite(Motor::pin2, speed>0?0:-speed);
    }
  private:
    int pin1, pin2;
};

class MyMotorShield {
  public:
    Motor motor_left;
    Motor motor_right;
    void setup() {
      MyMotorShield::motor_left.setup(MOTOR_DIR_A_1,MOTOR_DIR_A_2);
      MyMotorShield::motor_right.setup(MOTOR_DIR_B_1,MOTOR_DIR_B_2);
    }
    void motors(int speed_left, int speed_right) {
      MyMotorShield::motor_left.run(speed_left);
      MyMotorShield::motor_right.run(speed_right);
    }
    void stop_motors() {
      MyMotorShield::motors(0,0);
    }
};

MyMotorShield MotorShield;
