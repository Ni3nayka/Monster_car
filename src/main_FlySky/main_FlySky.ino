/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  March 2024
   modify: June 2025
*/

// #define WIFI_BOOTLOAD
#ifdef WIFI_BOOTLOAD
#define ENABLE_AVOCADO_ESP_WIFI_BOOT
#include "user_data.h"
#include <AVOCADO_esp.h> 
AVOCADO_esp esp;    
#endif

#include "motor.h"

#include "FlySky.h"
#define EXACT_FORWARD_K 0.3
#define TURN_K 0.35
unsigned long int main_flysky_time = 0;


#include "SoftwareSerial.h"
SoftwareSerial servoDriver(5, 18); // RX, TX
#define GY_25_SERIAL servoDriver

#include "gy-25-parser.h"
GY25Parser gy25;

int global_stick_l_old = -1, global_stick_r_old = -1;
unsigned long int servo_update_time = 0;

// long int global_integral = 0;
int global_e_old = 0;

void setup(){
  gy25.setup();

  Serial.begin(115200);
  MotorShield.setup();
  FlySky.begin(Serial2);
  servoDriver.begin(9600);
  
  #ifdef WIFI_BOOTLOAD
  if (digitalRead(ENABLE_BUTTON)==1) {
    esp.setup();
  }
  else {
    for (int i = 0; i<5; i++) {
      digitalWrite(ENABLE_LED,1);
      delay(100);
      digitalWrite(ENABLE_LED,0);
      delay(100);
    }
  }
  #endif
  
  MotorShield.run(10, 10);
  delay(1000);
  MotorShield.run();
}

void loop() { 
  mainFlySky();
  gy25.parseData();
  // esp.update(); 
}

void mainFlySky() {
  // for (int i = 0; i<14; i++) Serial.print(String(FlySky.readChannel(i)) + " "); Serial.println();

  int joystick_left_y = FlySky.readChannel(2);
  int joystick_left_x = FlySky.readChannel(3);
  int joystick_right_y = FlySky.readChannel(1);
  int joystick_right_x = FlySky.readChannel(0);
  int stick_l = FlySky.readChannel(8);
  int stick_r = FlySky.readChannel(9);
  int swa = FlySky.readChannel(4)>0;
  int swb = FlySky.readChannel(5)>0;
  int swc = FlySky.readChannel(6); swc = swc==0?0:(swc>0?1:-1);
  int swd = FlySky.readChannel(7)>0;

  if (swb) {

    int forward = joystick_right_y*EXACT_FORWARD_K + (joystick_left_y+100)/2;
    int turn = (joystick_left_x + joystick_right_x)*TURN_K;
    // Serial.println(String(left_speed) + " " + String(right_speed));
    // Serial.println(String(stick_l) + " " + String(stick_r));
    if (swd==1) {
      forward = joystick_right_y + (joystick_left_y+100)/2;
      turn = joystick_right_x;
    }
    int left_speed = forward + turn;
    int right_speed = forward - turn;
    
    stick_l = map(stick_l,-100,100,0,180);
    stick_r = map(stick_r,-100,100,0,180);
    if (global_stick_l_old!=stick_l || global_stick_r_old!=stick_r || servo_update_time<millis()) {
      servoDriver.println(String(stick_l)+"1");
      // Serial.println("[" + String(stick_l) + " " + String(stick_r) + " ]");
      servo_update_time = millis()+1000;
    }
    global_stick_l_old = stick_l;
    global_stick_r_old = stick_r;

    if (swc==-1) {
      MotorShield.run(left_speed, right_speed);
    }
    else if (swc==0) { // auto_gyroscope
      
    }
    else { // auto_black_line
      float K = 0.2;
      long int e = (analogRead(35)-2100)*K;
      int M = 28;
      // Serial.println(e);
      int l = constrain(M+e, -M, M);
      int r = constrain(M-e, -M/2, M);
      MotorShield.run(l, r);
    }
    
    // servo_1.write(map(stick_l,-100,100,0,180));
    // servo_2.write(map(stick_r,-100,100,0,180));
  }
  else {
    MotorShield.run();
  }

  delay(10); //   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!
}
