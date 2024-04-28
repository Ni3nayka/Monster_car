#include "motor.h"
#include "hc-sr04.h"

#define MOTOR_SPEED 20

void setup(){
  Serial.begin (9600);
  MotorShield.setup();
  // MotorShield.run(100,100);
  // delay(1000);
  MotorShield.run();
  setupUltrasonars();
}


void loop(){
  // Serial.print(readUltrasonar(1));
  // Serial.print(" ");
  // Serial.println(readUltrasonar(2));

  int e = -readUltrasonar(1) + readUltrasonar(2);
  int pid = e*1;

  // Serial.println(pid);
  MotorShield.run(MOTOR_SPEED+pid,MOTOR_SPEED-pid);
}

