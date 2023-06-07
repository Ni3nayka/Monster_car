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

class MyServo {
  public:
    //int reverse;
    void attach(int pin, int min_time=SERVO_MIN_TIME, int max_time=SERVO_MAX_TIME, int min_angle=SERVO_MIN_ANGLE, int max_angle=SERVO_MAX_ANGLE) {
      MyServo::pin = pin;
      pinMode(MyServo::pin,OUTPUT);
      MyServo::min_time = min_time;
      MyServo::max_time = max_time;
      MyServo::min_angle = min_angle;
      MyServo::max_angle = max_angle;
    }
    void write(int angle=-9999) {
      if (angle!=-9999) MyServo::angle = constrain(angle,min_angle,max_angle);
      int pos = map(MyServo::angle,min_angle,max_angle,min_time,max_time);
      digitalWrite(MyServo::pin, HIGH);
      delayMicroseconds(pos);
      digitalWrite(MyServo::pin, LOW);
      delayMicroseconds(20000 - pos);
      delay(1);
    }
    void write_on(int d_angle) {
      MyServo::write(MyServo::angle+d_angle);
    }
    int get_angle() {
      return MyServo::angle;
    }
  private:
    int pin,min_time,max_time,min_angle,max_angle,angle;
};
