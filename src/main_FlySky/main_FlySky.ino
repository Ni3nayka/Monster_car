/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  March 2024
   modify: March 2024
*/

#include "lazer.h"
Lazer lazer_1(0x30,18);
Lazer lazer_2(0x31,19);

#define WIFI_BOOTLOAD
#ifdef WIFI_BOOTLOAD
#define ENABLE_AVOCADO_ESP_WIFI_BOOT
#include "user_data.h"
#include <AVOCADO_esp.h> 
AVOCADO_esp esp;    
#endif

// #include "FlySky.h"
#include "motor.h"

// #define EXACT_FORWARD_K 0.3
// #define TURN_K 0.2

#define ENABLE_LED 23
#define ENABLE_BUTTON 34
bool global_enable_button = 0;

// long int global_integral = 0;
int global_e_old = 0;

void setup(){
  Serial.begin(115200);
  MotorShield.setup();
  firstSetupLazers();
  lazer_1.setup();
  lazer_2.setup();
  
  #ifdef WIFI_BOOTLOAD
  esp.setup();
  #endif
  
  // button
  pinMode(ENABLE_BUTTON, INPUT);
  pinMode(ENABLE_LED,OUTPUT);
  digitalWrite(ENABLE_LED,1);
  // FlySky.begin(Serial); // Serial2

  // MotorShield.run(100,100);
  // delay(1000);
  // MotorShield.run(100,0); delay(1000);
  // MotorShield.run(-100,0); delay(1000);
  // MotorShield.run(30,0); delay(1000);
  // MotorShield.run(-30,0); delay(1000);
  // MotorShield.run(0,100); delay(1000);
  // MotorShield.run(0,-100); delay(1000);
  // MotorShield.run();
}

void loop() { 
  // Serial.println(String(lazer_1.get()) + " " + String(lazer_2.get())); delay(100);
  if (global_enable_button) mainKoridor();
  else esp.update(); 
  if (digitalRead(ENABLE_BUTTON)==0) {
    digitalWrite(ENABLE_LED,0);
    MotorShield.run();
    while ((digitalRead(ENABLE_BUTTON)==0));
    delay(100);
    global_enable_button = !global_enable_button;
  }
  // esp.update(); 
}

void mainKoridor() {
  int m = 30;
  float k_pid_backward = 1.0;
  float k_pid_forward = 0.2;

  int e = lazer_1.get() - lazer_2.get();
  int p = e;
  // long int kub = e*e*e;
  // global_integral = global_integral*0.7 + e;
  int d = e-global_e_old;
  global_e_old = e;

  long int pid = 0;
  pid += p*0.13; // 0.13
  // pid += global_integral*0.07;
  // pid += kub*0.008;
  // pid += d*1; 
  // Serial.println(pid);
  
  int pid_forward = constrain(abs(pid),0,m*k_pid_forward);
  int pid_backward = constrain(abs(pid),0,m*k_pid_backward);
  if (pid<0) MotorShield.run(m+pid_forward,m-pid_backward);
  else MotorShield.run(m-pid_backward,m+pid_forward);
  // MotorShield.run(m-pid,m+pid);
}

void mainFlySky() {
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
}

