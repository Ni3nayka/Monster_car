#include "lazer.h"

Lazer lazer_1(0x30,18);
Lazer lazer_2(0x31,19);

void setup() {
  Serial.begin(115200);
  firstSetupLazers();
  lazer_1.setup();
  lazer_2.setup();
}

void loop() {
  Serial.println(String(lazer_1.get()) + " " + String(lazer_2.get()));
  delay(100);
}