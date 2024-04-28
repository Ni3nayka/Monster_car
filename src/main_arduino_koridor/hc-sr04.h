#pragma once

#define PIN_TRIG_1 6
#define PIN_ECHO_1 7
#define PIN_TRIG_2 12
#define PIN_ECHO_2 8

void setupUltrasonars() {
  //Определяем вводы и выводы
  pinMode(PIN_TRIG_1, OUTPUT);
  pinMode(PIN_ECHO_1, INPUT);
  pinMode(PIN_TRIG_2, OUTPUT);
  pinMode(PIN_ECHO_2, INPUT);
}

int readUltrasonarBasic(int pin_trig, int pin_echo) {
  int duration, cm;

  // Сначала генерируем короткий импульс длительностью 2-5 микросекунд.

  digitalWrite(pin_trig, LOW);
  delayMicroseconds(2); // 5
  digitalWrite(pin_trig, HIGH);

  // Выставив высокий уровень сигнала, ждем около 10 микросекунд. В этот момент датчик будет посылать сигналы с частотой 40 КГц.
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW);

  //  Время задержки акустического сигнала на эхолокаторе.
  duration = pulseIn(pin_echo, HIGH);

  // Теперь осталось преобразовать время в расстояние
  cm = (duration / 2) / 29.1;

  // Serial.print("Расстояние до объекта: ");
  // Serial.print(cm);
  // Serial.println(" см.");

  // // Задержка между измерениями для корректной работы скеча
  // delay(250);
  cm = constrain(cm,0,100);
  return cm;
}

int readUltrasonar(int number) {
  if (number==1) return readUltrasonarBasic(PIN_TRIG_1,PIN_ECHO_1);
  if (number==2) return readUltrasonarBasic(PIN_TRIG_2,PIN_ECHO_2);
  return 0;
}
