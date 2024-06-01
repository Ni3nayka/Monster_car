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
#define MOTOR_PWM_HZ 30000 // частота ШИМ
#define MOTOR_BITS 8 // разрядность (для управления) ШИМ
#define MOTOR_MAP 2.5 // для перевода скорости из размерности 100 в размерность в соответствии с MOTOR_BITS (8 бит => 255 "ШИМ" ~ 250)
int global_channel_pwm = 0;

class Motor {
  public:
    //int reverse;
    void setup(int pin1, int pin2, int pin_en) {
      Motor::pin1 = pin1;
      Motor::pin2 = pin2;
      Motor::pin_en = pin_en;
      pinMode(Motor::pin_en,OUTPUT);
      ledcSetup(global_channel_pwm, MOTOR_PWM_HZ, MOTOR_BITS);
      ledcSetup(global_channel_pwm+1, MOTOR_PWM_HZ, MOTOR_BITS);
      ledcAttachPin(Motor::pin1, global_channel_pwm);
      ledcAttachPin(Motor::pin2, global_channel_pwm+1);
      Motor::channel = global_channel_pwm;
      global_channel_pwm+=2;
      Motor::run();
    }
    void run(int speed=0) {
      speed = constrain(speed,-100,100)*MOTOR_MAP;
      digitalWrite(Motor::pin_en,speed!=0);
      ledcWrite(Motor::channel, constrain(speed,0,255));
      ledcWrite(Motor::channel+1, constrain(-speed,0,255));
    }
  private:
    int pin1, pin2, pin_en, channel;
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
