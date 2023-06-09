/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  June 2022
   modify: June 2023
*/

#pragma once

#define SERVO_MIN_TIME 540
#define SERVO_MAX_TIME 2400
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_NONE -9999
//#define SERVO_MULTITHREAD // для многопоточности (внешней)

class MyServo {
  public:
    int pos;
    void attach(int pin, int min_time=SERVO_MIN_TIME, int max_time=SERVO_MAX_TIME, int min_real_angle=SERVO_MIN_ANGLE, int max_real_angle=SERVO_MAX_ANGLE, int min_angle=SERVO_NONE, int max_angle=SERVO_NONE) {
      // min_real_angle - потенциально возможные углы сервы
      // min_angle - насколько ограничить из предыдущего диапазона
      MyServo::pin = pin;
      pinMode(MyServo::pin,OUTPUT);
      MyServo::min_time = min_time;
      MyServo::max_time = max_time;
      MyServo::min_real_angle = min_real_angle;
      MyServo::max_real_angle = max_real_angle;
      if (min_angle==SERVO_NONE) MyServo::min_angle = min_real_angle;
      else MyServo::min_angle = min_angle;
      if (max_angle==SERVO_NONE) MyServo::max_angle = max_real_angle;
      else MyServo::max_angle = max_angle;
    }
    void write(int angle=SERVO_NONE) { // сделать многопоточно
      if (angle!=SERVO_NONE) MyServo::angle = constrain(angle,MyServo::min_angle,MyServo::max_angle);
      MyServo::pos = map(MyServo::angle,MyServo::min_real_angle,MyServo::max_real_angle,MyServo::min_time,MyServo::max_time);
      #ifndef SERVO_MULTITHREAD
      digitalWrite(MyServo::pin, HIGH);
      delayMicroseconds(MyServo::pos);
      digitalWrite(MyServo::pin, LOW);
      delayMicroseconds(20000 - MyServo::pos);
      delay(1);
      #endif
    }
    void write_on(int d_angle) {
      MyServo::write(MyServo::angle+d_angle);
    }
    int get_angle() {
      return MyServo::angle;
    }
  private:
    int pin,min_time,max_time,min_real_angle,max_real_angle,min_angle,max_angle,angle;
};
