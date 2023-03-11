#include "gamepad.h"
#include "motor.h"

void setup(){
  Serial.begin(9600);
  gamepad.setup();
  setup_motor();
}

void loop(){
  gamepad.update();
  motor(LEFT,  gamepad.joystick(PSS_LY));
  motor(RIGHT, gamepad.joystick(PSS_RY));
  gamepad.vibration = gamepad.button(PSB_BLUE);
}
