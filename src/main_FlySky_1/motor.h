/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  May 2022
   modify: March 2024
*/

#pragma once

#define MOTOR_DIR_A_1 14
#define MOTOR_DIR_A_2 27
#define MOTOR_DIR_A_PWM 26
#define MOTOR_DIR_B_1 32
#define MOTOR_DIR_B_2 33
#define MOTOR_DIR_B_PWM 25


// устанавливаем настройки формирования ШИМ
#define MOTOR_PWM_HZ 15000 // частота ШИМ
#define MOTOR_BITS 8 // разрядность (для управления) ШИМ
#define MOTOR_MAP 2.5 // для перевода скорости из размерности 100 в размерность в соответствии с MOTOR_BITS (8 бит => 255 "ШИМ" ~ 250)
int global_channel_pwm = 0;

class Motor {
  public:
    //int reverse;
    void setup(int pin1, int pin2, int pin_pwm) {
      Motor::pin1 = pin1;
      Motor::pin2 = pin2;
      Motor::pin_pwm = pin_pwm;
      pinMode(Motor::pin1,OUTPUT);
      pinMode(Motor::pin2,OUTPUT);
      // pinMode(Motor::pin_pwm,OUTPUT);
      ledcSetup(global_channel_pwm, MOTOR_PWM_HZ, MOTOR_BITS);
      ledcAttachPin(Motor::pin_pwm, global_channel_pwm);
      Motor::channel = global_channel_pwm;
      global_channel_pwm++;
      // переключаем ШИМы в ноль(чтобы все стояло)
      Motor::run();
      //reverse = 1;
    }
    void run(int speed=0) {
      speed = constrain(speed,-100,100)*MOTOR_MAP;
      //speed *= Motor::reverse;
      // analogWrite(Motor::pin1, speed>0?speed:0);
      // analogWrite(Motor::pin2, speed>0?0:-speed);
      ledcWrite(Motor::channel, abs(speed));
      digitalWrite(Motor::pin1, speed>0);
      digitalWrite(Motor::pin2, speed<0);
    }
  private:
    int pin1, pin2, pin_pwm, channel;
};

class MyMotorShield {
  public:
    Motor motor_left;
    Motor motor_right;
    void setup() {
      MyMotorShield::motor_left.setup(MOTOR_DIR_A_1,MOTOR_DIR_A_2,MOTOR_DIR_A_PWM);
      MyMotorShield::motor_right.setup(MOTOR_DIR_B_1,MOTOR_DIR_B_2,MOTOR_DIR_B_PWM);
    }
    void run(int speed_left=0, int speed_right=0) {
      MyMotorShield::motor_left.run(speed_left);
      MyMotorShield::motor_right.run(speed_right);
    }
    void stop_motors() {
      MyMotorShield::run();
    }
};

MyMotorShield MotorShield;