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
#define TURN_K 0.3
unsigned long int main_flysky_time = 0;


// #include <ESP32Servo.h>
// Servo servo_1;
// Servo servo_2;
int global_stick_l_old = -1, global_stick_r_old = -1;

// long int global_integral = 0;
int global_e_old = 0;

void setup(){
  Serial.begin(9600);
  MotorShield.setup();
  FlySky.begin(Serial2);
  // firstSetupLazers();
  // lazer_1.setup();
  // lazer_2.setup();

  // servo_1.attach(14);
  // servo_2.attach(27);

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
  mainFlySky();
  // servoUpdate();
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
  // for (int i = 0; i<14; i++) Serial.print(String(FlySky.readChannel(i)) + " "); Serial.println();
  if (FlySky.readChannel(8)!=0) {
    int joystick_left_y = FlySky.readChannel(2);
    int joystick_left_x = FlySky.readChannel(3);
    int joystick_right_y = FlySky.readChannel(1);
    int joystick_right_x = FlySky.readChannel(0);
    int stick_l = FlySky.readChannel(9);
    int stick_r = FlySky.readChannel(10);
    int swa = FlySky.readChannel(6)>0;
    int swb = FlySky.readChannel(5)>0;
    int swc = FlySky.readChannel(4); swc = swc==0?0:(swc>0?1:-1);
    int swd = FlySky.readChannel(7)>0;

    int forward = joystick_right_y*EXACT_FORWARD_K + (joystick_left_y+100)/2;
    int turn = (joystick_left_x + joystick_right_x)*TURN_K;
    int left_speed = forward + turn;
    int right_speed = forward - turn;
    // Serial.println(String(left_speed) + " " + String(right_speed));
    // Serial.println(String(stick_l) + " " + String(stick_r));
    
    stick_l = map(stick_l,-100,100,0,90);
    stick_r = map(stick_r,-100,100,0,90);
    if (global_stick_l_old!=stick_l || global_stick_r_old!=stick_r) {
      Serial.println("[" + String(stick_l) + " " + String(stick_r) + " ]");
    }
    global_stick_l_old = stick_l;
    global_stick_r_old = stick_r;

    MotorShield.run(left_speed, right_speed);
    // servo_1.write(map(stick_l,-100,100,0,180));
    // servo_2.write(map(stick_r,-100,100,0,180));
  }

  // if (main_flysky_time>millis()) return;
  // main_flysky_time = millis() + 50;
  delay(10); //   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!   НУЖНО!!!
}

