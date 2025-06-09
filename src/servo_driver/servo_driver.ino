/*
формат данных:
(данные)+(номер устройства)
321 = устройство 1, положение 32

устройства:
1 - серво 1
2 - серво 2
7 - вывести в монитор данные по гироскопу (данные можно вообще не подавать)

*/


#include <Servo.h>
Servo servo_1;
Servo servo_2;

int global_object = 0;
int global_data = 0;

unsigned long int time = 0;

#include "GY-25.h"
GY25 gy25(A0,A1); // (TX,RX) - пины гироскопа

void setup() {
  Serial.begin(9600); 
  servo_1.attach(8);
  servo_2.attach(7);
  servo_1.write(80);
  servo_2.write(70);
  // Serial.println("Готов к приему данных...");
  gy25.setup(); // gy25.calibration();
}

void loop() {
  gy25.update();
  parseData();
  //testParseData();
  operations();
}

void testParseData() {
  //if (time < millis()) {
  if (global_object!=0) {
    Serial.print(global_object);
    Serial.print(" ");
    Serial.println(global_data);
    time = millis() + 1000;
  }
}

void operations() {
  if (global_object==1) servo_1.write(constrain(global_data,0,180));
  if (global_object==2) servo_2.write(constrain(global_data,0,180));
  if (global_object==7) gy25.println();
  global_object = 0;
  global_data = 0;
}

void parseData() {
  if (Serial.available() > 0) {
    static String inputString = ""; // Строка для накопления входных данных
    char inChar = Serial.read(); // Чтение символа
    
    if (inChar != '\n' && inChar != '\r') {
      // Если это не символ конца строки, добавляем его в строку
      inputString += inChar;
    } else {
      // Если получен символ новой строки, парсим число
      if (inputString.length() > 0) {
        // Пробуем преобразовать строку в число
        bool isNumber = true;
        for (int i = 0; i < inputString.length(); i++) {
          if (!isdigit(inputString[i])) {
            isNumber = false;
            break;
          }
        }
        
        if (isNumber) {
          int number = inputString.toInt();
          global_object = number%10;
          global_data = number/10;
        } 
        
        inputString = ""; // Сбрасываем строку для следующего ввода

      }
    }
  }
}