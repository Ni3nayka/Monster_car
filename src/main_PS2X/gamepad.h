/*
// main example
#include "gamepad.h"

void setup(){
  Serial.begin(9600);
  gamepad.setup();
}

void loop(){
  gamepad.update();
  for (int i=0; i<gamepad_button_quantity; i++) {
    if (gamepad.button_first_tap(gamepad_button_code[i])) {
      Serial.println(gamepad_button_name[i]);
    }
  }
  for (int i=0; i<gamepad_joystick_quantity; i++) {
    if (gamepad.joystick(gamepad_joystick_code[i])!=0) {
      Serial.print(gamepad_joystick_name[i]);
      Serial.print(" ");
      Serial.println(gamepad.joystick(gamepad_joystick_code[i]));
    }
  }
  gamepad.vibration = gamepad.button(PSB_BLUE);
}
*/

#pragma once
#include <Arduino.h>
#include "PS2X_lib.h"  //for v1.6

const uint8_t  PS2_clock          = A2;    // CLK
const uint8_t  PS2_data           = A0;    // DAT (DTA)
const uint8_t  PS2_command        = A1;    // CMD
const uint8_t  PS2_attention      = A3;    // ATT

const bool     PS2_Pressures      = true;  // установка флага на чтение аналоговых значений с геймпада
const bool     PS2_Rumble         = true;

const unsigned int gamepad_button_quantity = 16;
const unsigned int gamepad_button_code[gamepad_button_quantity] = {PSB_START,PSB_SELECT,PSB_PAD_UP,PSB_PAD_RIGHT,PSB_PAD_LEFT,PSB_PAD_DOWN,PSB_L2,PSB_R2,PSB_GREEN,PSB_RED,PSB_PINK,PSB_BLUE,PSB_L1,PSB_R1,PSB_L3,PSB_R3};
const String gamepad_button_name[gamepad_button_quantity] = {"PSB_START","PSB_SELECT","PSB_PAD_UP","PSB_PAD_RIGHT","PSB_PAD_LEFT","PSB_PAD_DOWN","PSB_L2","PSB_R2","PSB_GREEN","PSB_RED","PSB_PINK","PSB_BLUE","PSB_L1","PSB_R1","PSB_L3","PSB_R3"};

const unsigned int gamepad_joystick_quantity = 4;
const unsigned int gamepad_joystick_code[gamepad_joystick_quantity] = {PSS_LY,PSS_LX ,PSS_RY,PSS_RX};
const String gamepad_joystick_name[gamepad_joystick_quantity] = {"PSS_LY","PSS_LX","PSS_RY","PSS_RX"};

/*
кнопки:

PSB_START
PSB_SELECT

PSB_PAD_UP
PSB_PAD_RIGHT
PSB_PAD_LEFT
PSB_PAD_DOWN

PSB_GREEN  // треугольник
PSB_RED    // куруг
PSB_PINK   // квадрат
PSB_BLUE   // крест

PSB_L1
PSB_R1
PSB_L2
PSB_R2

джойстик:

PSB_L3
PSB_R3
PSS_LY
PSS_LX
PSS_RY
PSS_RX
*/

class GamePad {
  public:
    void setup();
    void update(int vibro=-1);
    int joystick(unsigned int number);
    bool button(unsigned int number);
    bool button_first_tap(unsigned int number);
    bool vibration = 0;
    PS2X _ps2x; // create PS2 Controller Class
  private:
    int _error = 5;
};

GamePad gamepad;

void GamePad::setup(){
  Serial.print("connecting with joystick");
  while (_error){
    _error = _ps2x.config_gamepad(PS2_clock,PS2_command,PS2_attention,PS2_data, PS2_Pressures, PS2_Rumble);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("joystick connect");
}

void GamePad::update(int vibro=-1) {
  if (vibro>=0) vibration = vibro;
  if(_error) return;
  _ps2x.read_gamepad(false, vibration*255);
  delay(50);
}

bool GamePad::button(unsigned int number) {
  if(_error) return 0;
  return _ps2x.Button(number);
}

bool GamePad::button_first_tap(unsigned int number) {
  if(_error) return 0;
  return _ps2x.ButtonPressed(number);
}

int GamePad::joystick(unsigned int number) {
  if(_error) return;
  int t = _ps2x.Analog(number)-127;
  if (abs(t)<3) t = 0;
  else {
    if (number==PSS_LY||number==PSS_RY) t *= -1;
    t = map(t,-128,127,-100,100);
  }
  return t;
}
