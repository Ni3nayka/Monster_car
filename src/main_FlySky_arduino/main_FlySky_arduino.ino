/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  March 2024
   modify: March 2024
*/

#include "FlySky.h"
#include "motor.h"

#define EXACT_FORWARD_K 0.3
#define TURN_K 0.2


#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup(){
  Serial.begin(115200);
  // FlySky.begin(Serial); // Serial2
  MotorShield.setup();
  // MotorShield.run(100,100);
  // delay(1000);
  MotorShield.run();

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}


void loop(){
  // // for (int i = 0; i<14; i++) Serial.print(String(FlySky.readChannel(i)) + " "); Serial.println();
  // int joystick_left_y = FlySky.readChannel(2);
  // int joystick_left_x = FlySky.readChannel(3);
  // int joystick_right_y = FlySky.readChannel(1);
  // int joystick_right_x = FlySky.readChannel(0);
  // int forward = joystick_right_y*EXACT_FORWARD_K + (joystick_left_y+100)/2;
  // int turn = (joystick_left_x + joystick_right_x)*TURN_K;
  // int left_speed = forward + turn;
  // int right_speed = forward - turn;
  // // Serial.println(String(left_speed) + " " + String(right_speed));
  // MotorShield.motors(left_speed, right_speed);
  // delay(10);


  VL53L0X_RangingMeasurementData_t measure;
    
  // Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
  int dist = 0;

  int max = 600;
  int center = 300;

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    // Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
    dist = constrain(measure.RangeMilliMeter,0,max);
  } else {
    // Serial.println(" out of range ");
    measure.RangeMilliMeter = max;
  }
  Serial.println(dist);
    
  delay(10);

  dist = (dist-center)*0.3;

  MotorShield.run(50+dist,50-dist);
}
