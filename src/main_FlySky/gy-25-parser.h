/*
   code write for project:
   https://github.com/Ni3nayka/Monster_car

   author: Egor Bakay <egor_bakay@inbox.ru> Ni3nayka
   write:  June 2025
   modify: June 2025
*/

#pragma once

#define GY_25_DATA_NONE -1
#define GY_25_DATA_SIZE 3

#ifndef GY_25_SERIAL
#define GY_25_SERIAL Serial
#endif

class GY25Parser {
  public:

    int data[GY_25_DATA_SIZE];
    bool recieved_data = 0;

    void setup() {
      GY25Parser::clearGyroscopeData();
    }

    void parseData() {
      if (GY_25_SERIAL.available() > 0) {
        static String inputString;// = ""; // Строка для накопления входных данных
        char inChar = GY_25_SERIAL.read(); // Чтение символа
        // Serial.println(inChar);
        // Serial.println(inputString);
        
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
              Serial.print(" ");
              Serial.println(number);
              GY25Parser::writeGyroscopeData(number);
            } 
            
            inputString = ""; // Сбрасываем строку для следующего ввода

          }
        }
      }
    }

    void requestData() {
      GY_25_SERIAL.println("7");
    }

  private:

    void clearGyroscopeData() {
      for (int i = 0; i<GY_25_DATA_SIZE; i++) {
        GY25Parser::data[i] = GY_25_DATA_NONE;
      }
      GY25Parser::recieved_data = 0;
    }

    void writeGyroscopeData(int number) {
      for (int i = 0; i<GY_25_DATA_SIZE; i++) {
        if (GY25Parser::data[i]==GY_25_DATA_NONE) {
          GY25Parser::data[i] = number;
          if (GY_25_DATA_SIZE==i+1) {
            GY25Parser::recieved_data = 1;
          }
          return;
        }
      }
      GY25Parser::clearGyroscopeData();
      GY25Parser::writeGyroscopeData(number);
    }
};

