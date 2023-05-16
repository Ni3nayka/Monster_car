#include <esp8266_wifi_Serial.h>
const int counter_servo = 6 ;
const int pins_servo[counter_servo] = {D5, D2, D1, D0, D6, D7};
//const int pins_servo[counter_servo] = {2, 3, 4, 5, 6, 7};
int angle_servo [counter_servo] = {90, 90, 90, 90, 90, 90};

const int cointer_alphabet = 60;
String alphabet = "0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvb";

void setup() {
  Serial.begin(9600);
  WifiSerial.begin("SERVER");
  for (int i = 0; i < counter_servo; i++)
  {
    pinMode(pins_servo[i], OUTPUT);
  }
}
void loop() {
  Read_data ();
  upgrade_servoes ();
}

void write_servo(int pin, int angle) {
  angle = constrain(angle, 0, 180);
  int pos = map(angle, 0, 180, 544, 2400);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pos);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000 - pos);
  delay(1);

}
void upgrade_servoes() {
  for (int i = 0; i < counter_servo; i++) {
    Serial.print (angle_servo [i]);
    Serial.print( " ");
    write_servo(pins_servo[i], angle_servo[i]);
  } Serial.println( " ");
}
int AAA = 0;
int z = 0;
void Read_data ()
{
  if (WifiSerial.available() != 0) {
    int y = WifiSerial.read();
    z = -1;
    for (int i = 0; i < cointer_alphabet; i++) {
      if (y == alphabet[i]) {
        z = i * 3;
      }
    } // Serial.println ( z);
    if (z != -1 ) {
      angle_servo [AAA] = z;
      AAA++;
      if (AAA >= 6) {
        AAA = 0;
      }
    }
  }
}
