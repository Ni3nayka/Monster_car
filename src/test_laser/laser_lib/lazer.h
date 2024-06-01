// https://github.com/adafruit/Adafruit_VL53L0X/

#pragma once

// #include "lazer.h"
#include "Adafruit_VL53L0X.h"

#define DISABLE_PIN_DEFAULT -1
#define ADDRESS_DEFAULT -1
#define MAX_LAZER_COUNT 10
#define MAX_LAZER_DISTANCE 1000

int global_lazer_disable_pins[MAX_LAZER_COUNT] = {0};
int global_lazer_counter = 0;

class Lazer {
  public:
    Adafruit_VL53L0X lox = Adafruit_VL53L0X();
    Lazer(int address = ADDRESS_DEFAULT, int disable_pin=DISABLE_PIN_DEFAULT) {
      global_lazer_disable_pins[global_lazer_counter] = disable_pin;
      global_lazer_counter++;
      Lazer::disable_pin = disable_pin;
      Lazer::enable = 0;
      Lazer::address = address;
    }
    void setup() {
      if (disable_pin==DISABLE_PIN_DEFAULT) {
        Lazer::enable = lox.begin();
      } 
      else {
        // start this sensor
        digitalWrite(Lazer::disable_pin,1);
        delay(20);
        Lazer::enable = lox.begin(Lazer::address);
      }
      if (!Lazer::enable) Serial.println(F("Failed to boot VL53L0X"));
    }
    int get() {
      VL53L0X_RangingMeasurementData_t measure;
      Lazer::lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
      if(measure.RangeStatus != 4) {
        return constrain(measure.RangeMilliMeter,0,MAX_LAZER_DISTANCE);
      } 
      else {
        return MAX_LAZER_DISTANCE;
      }
    }
  private:
    int disable_pin, address;
    bool enable;
};

void firstSetupLazers() {
  for (int i = 0; i<global_lazer_counter; i++) pinMode(global_lazer_disable_pins[i], OUTPUT);
  // all reset
  for (int i = 0; i<global_lazer_counter; i++) digitalWrite(global_lazer_disable_pins[i], 0);
  delay(10);
  // all unreset
  for (int i = 0; i<global_lazer_counter; i++) digitalWrite(global_lazer_disable_pins[i], 1);
  delay(10);
  // resetting all
  for (int i = 0; i<global_lazer_counter; i++) digitalWrite(global_lazer_disable_pins[i], 0);
  delay(10);
}