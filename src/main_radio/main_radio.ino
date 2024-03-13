#include <iarduino_RF433_Receiver.h>
iarduino_RF433_Receiver radio(2); // (пин)

int data[5]; // массив для обмена данными

void setup(){
  radio.begin();
  radio.setDataRate(i433_1KBPS); // Указываем скорость приёма данных (i433_5KBPS, i433_4KBPS, i433_3KBPS, i433_2KBPS, i433_1KBPS, i433_500BPS, i433_100BPS), i433_1KBPS - 1кбит/сек
  radio.openReadingPipe(5); // Открываем 5 трубу (от 0 до 7)
  radio.startListening();
  Serial.begin(9600);
}
void loop(){
  if(radio.available()){
    radio.read(&data, sizeof(data));
    for (int i = 0; i<5; i++) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
