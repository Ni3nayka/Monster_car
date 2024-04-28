#include "motor.h"

#define MOTOR_SPEED 20

#define IK_MIN 5
#define IK_MAX 80


void setup(){
  Serial.begin(9600);
  MotorShield.setup();
  // MotorShield.run(100,100);
  // delay(1000);
  MotorShield.run();
}

int getIK(int port) {
  return constrain(32*pow(analogRead(port)*0.0048828125, -1.10),IK_MIN,IK_MAX);
}

void loop(){
  // Serial.print(getIK(A0));
  // Serial.print(" ");
  // Serial.print(getIK(A2));
  // Serial.println();

  int e = getIK(A2) - getIK(A0);
  int pid = e*0.5;

  // Serial.println(pid);
  MotorShield.run(MOTOR_SPEED+pid,MOTOR_SPEED-pid);
}

