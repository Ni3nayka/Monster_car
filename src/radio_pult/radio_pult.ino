#define RADIO_PIN 2
#include <iarduino_RF433_Transmitter.h>
iarduino_RF433_Transmitter radio(RADIO_PIN);

#define CHANNEL_BUTTON_COUNT 3
const int CHANNEL_BUTTONS[CHANNEL_BUTTON_COUNT] = {6,5,4};

#define BUTTON_COUNT 3
const int BUTTON_PINS[BUTTON_COUNT] = {7,8,9};

#define JOYSTICK_COUNT 4
const int JOYSTICK_PINS[JOYSTICK_COUNT] = {A0,A1,A2,A3};

const int DATA_SIZE = (JOYSTICK_COUNT+1)/2+1;
int data[DATA_SIZE]; // массив для обмена данными

void setup(){
  radio.begin();
  radio.setDataRate(i433_1KBPS); // Указываем скорость приёма данных (i433_5KBPS, i433_4KBPS, i433_3KBPS, i433_2KBPS, i433_1KBPS, i433_500BPS, i433_100BPS), i433_1KBPS - 1кбит/сек
  radio.openWritingPipe(read_channel()); // Открываем трубу (от 0 до 7)
  for (int i = 0; i < BUTTON_COUNT; i++) pinMode(BUTTON_PINS[i],INPUT_PULLUP);
  // Serial.begin(9600);
}

void loop(){
  for (int i = 0; i < JOYSTICK_COUNT; i++) {
    // data[i] = analogRead(JOYSTICK_PINS[i])>>2;
    int a = analogRead(JOYSTICK_PINS[i])>>2;
    data[i/2] = i%2==1?(data[i/2]+(a<<8)):a;
    // Serial.println(a);
  }
  int button = 0;
  for (int i = 0; i < BUTTON_COUNT; i++) button += int(!digitalRead(BUTTON_PINS[i]))<<i;
  data[DATA_SIZE-1] = button;
  radio.write(&data, sizeof(data));
  // for (int i = 0; i < DATA_SIZE; i++) Serial.print(String(data[i]) + " "); Serial.println(); delay(500);
}

int read_channel() {
  int channel = 0;
  for (int i = 0; i < CHANNEL_BUTTON_COUNT; i++) {
    pinMode(CHANNEL_BUTTONS[i],INPUT_PULLUP);
    channel += int(!digitalRead(CHANNEL_BUTTONS[i]))<<i;
  }
  return channel;
}