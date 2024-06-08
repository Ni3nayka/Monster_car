bool start_message = 0;
int number_message = 0;
int cache_message = 0;

#include <Servo.h>
Servo servo_1;
Servo servo_2;

// #include "gy-25.h"
// GY25 gy25(11,12); // (TX,RX) - пины гироскопа
// long int t = 0;

void setup() {
  Serial.begin(9600);
  // gy25.setup();
  // gy25.calibration();
  servo_1.attach(8);
  servo_2.attach(7);
  servo_1.write(80);
  servo_2.write(70);
}

void loop() {
  // gy25.update();
  // if (t<millis()) {
  //   gy25.print();
  //   t = millis()+50;
  // }
  
  if (Serial.available()) {
    char f = Serial.read();
    if (f=='[') {
      start_message = 1;
      number_message = 0;
      cache_message = 0;
    }
    else if (f==']') start_message = 0;
    else if (start_message==1) {
      if (f=='0') cache_message = cache_message*10;
      if (f=='1') cache_message = cache_message*10+1;
      if (f=='2') cache_message = cache_message*10+2;
      if (f=='3') cache_message = cache_message*10+3;
      if (f=='4') cache_message = cache_message*10+4;
      if (f=='5') cache_message = cache_message*10+5;
      if (f=='6') cache_message = cache_message*10+6;
      if (f=='7') cache_message = cache_message*10+7;
      if (f=='8') cache_message = cache_message*10+8;
      if (f=='9') cache_message = cache_message*10+9;
      if (f==' ') {
        cache_message *= 2;
        if (number_message==0) servo_1.write(cache_message);
        if (number_message==1) servo_2.write(cache_message);
        number_message++;
        cache_message = 0;
      }
    }
  }
}
