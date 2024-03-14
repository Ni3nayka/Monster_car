/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  Match 2024
   modify: Match 2024
*/

#include "motor.h"
#include "radio.h"

#include <iarduino_RF433_Receiver.h>
iarduino_RF433_Receiver radio(2); // (пин)

int data[3]; // массив для обмена данными

void setup(){
  Serial.begin(9600);
  MotorShield.setup();
  MotorShield.motors(0,0);
  radio.begin();
  radio.setDataRate(i433_1KBPS); // Указываем скорость приёма данных (i433_5KBPS, i433_4KBPS, i433_3KBPS, i433_2KBPS, i433_1KBPS, i433_500BPS, i433_100BPS), i433_1KBPS - 1кбит/сек
  radio.openReadingPipe(5); // Открываем 5 трубу (от 0 до 7)
  radio.startListening();
}
void loop(){
  if(radio.available()){
    radio.read(&data, sizeof(data));
    int joystick[4] = {0};
    for (int i = 0; i < 4; i++) joystick[i] = (data[i/2]>>(i%2?8:0))&0b11111111;
    for (int i = 0; i<4; i++) {
      joystick[i] = constrain(joystick[i]*2 - 255,-255,255);
      if (abs(joystick[i])<25) joystick[i] = 0;
    }
    for (int i = 0; i<4; i++) Serial.print(String(joystick[i]) + " "); Serial.println();
    int l = -joystick[1]-joystick[0];
    int r = -joystick[1]+joystick[0];
  }
}
