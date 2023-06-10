/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  June 2022
   modify: June 2023
*/

#pragma once

#include <Servo.h>

#define SERVO_MIN_TIME 540
#define SERVO_MAX_TIME 2400
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

class ServoPlus {
  public:
    Servo servo;
    void attach(int pin, int min_time=SERVO_MIN_TIME, int max_time=SERVO_MAX_TIME, int min_angle=SERVO_MIN_ANGLE, int max_angle=SERVO_MAX_ANGLE) {
      // min_real_angle - потенциально возможные углы сервы
      // min_angle - насколько ограничить из предыдущего диапазона
      ServoPlus::servo.attach(pin,min_time,max_time);
      ServoPlus::min_angle = min_angle;
      ServoPlus::max_angle = max_angle;
    }
    void write(int angle) { // сделать многопоточно
      angle = constrain(angle,ServoPlus::min_angle,ServoPlus::max_angle);
      ServoPlus::servo.write(angle);
    }
    void write_on(int d_angle) {
      ServoPlus::write(ServoPlus::read()+d_angle);
    }
    int read() {
      return ServoPlus::servo.read();
    }
  private:
    int min_angle,max_angle;
};
