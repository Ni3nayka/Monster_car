
#include "lazer.h"

void setup() {
  Serial.begin(115200);
  setup_lazer();
}

void loop() {
  Serial.print(getLazer(1));
  Serial.print(" ");
  Serial.print(getLazer(2));
  Serial.print(" ");
  Serial.print(getLazer(3));
  Serial.println();
  delay(100);
}