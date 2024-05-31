// https://github.com/adafruit/Adafruit_VL53L0X/


#pragma once

#include "Adafruit_VL53L0X.h"

#define MAX_LAZER_MEANING 1500 // 

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

// set the pins to shutdown
#define SHT_LOX1 13
#define SHT_LOX2 18
#define SHT_LOX3 5

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;


/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
 */

void setup_lazer() {
  // wait until serial port opens for native USB devices
  // while (! Serial) { delay(1); }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);

  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);

  Serial.println(F("Both in reset mode...(pins are low)"));
  
  
  Serial.println(F("Starting..."));
  // setID();

  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);    
  digitalWrite(SHT_LOX3, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  // my
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);    
  digitalWrite(SHT_LOX3, LOW);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  // digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println("Failed to boot 1 VL53L0X");
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println("Failed to boot 2 VL53L0X");
    while(1);
  }

  // activating LOX2
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX3_ADDRESS)) {
    Serial.println("Failed to boot 3 VL53L0X");
    while(1);
  }

  Serial.println("lazer success");
}

int getLazer(int number) {
  
  if (number==1) {
    lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
    if(measure1.RangeStatus != 4) {     // if not out of range
      return constrain(measure1.RangeMilliMeter,0,MAX_LAZER_MEANING);
    } else return MAX_LAZER_MEANING;
  }

  if (number==2) {
    lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
    if(measure2.RangeStatus != 4) {     // if not out of range
      return constrain(measure2.RangeMilliMeter,0,MAX_LAZER_MEANING);
    } else return MAX_LAZER_MEANING;
  }

  if (number==3) {
    lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
    if(measure3.RangeStatus != 4) {     // if not out of range
      return constrain(measure3.RangeMilliMeter,0,MAX_LAZER_MEANING);
    } else return MAX_LAZER_MEANING;
  }
}