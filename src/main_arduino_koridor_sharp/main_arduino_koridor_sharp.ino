#include "motor.h"

#define MOTOR_SPEED 20

#define IK_MIN 5
#define IK_MAX 90
#define IK_MIN_BIG 5
#define IK_MAX_BIG 150

#define FILTER_DATA_SIZE 3
class Filter {
  public:
    void setup() {
      for (int i = 0; i<FILTER_DATA_SIZE; i++) Filter::data[i] = 0;
    }
    void addData(int data) {
      for (int i = 0; i<FILTER_DATA_SIZE-1; i++) Filter::data[i] = Filter::data[i+1];
      Filter::data[FILTER_DATA_SIZE-1] = data;
    }
    int getData() {
      long int answer = 0;
      for (int i = 0; i<FILTER_DATA_SIZE; i++) answer += Filter::data[i];
      return answer/FILTER_DATA_SIZE;
    }
    int addGetData(int data) {
      Filter::addData(data);
      return Filter::getData();
    }
  private:
    int data[FILTER_DATA_SIZE];
};
Filter filter_0, filter_1, filter_2; 

void setup(){
  Serial.begin(9600);
  MotorShield.setup();

  filter_0.setup();
  filter_1.setup();
  filter_2.setup(); 

  // MotorShield.run(100,100);
  // delay(1000);
  MotorShield.run();

  for (int i = 0; i<FILTER_DATA_SIZE; i++) {
    filter_0.addData(getIKBig(A0));
    filter_1.addData(getIK(A1));
    filter_2.addData(getIKBig(A2));
  }
}

int getIK(int port) {
  return constrain(32*pow(analogRead(port)*0.0048828125, -1.10),IK_MIN,IK_MAX);
}


int getIKBig(int port) {
  return constrain(64*pow(analogRead(port)*0.0048828125, -1.10),IK_MIN_BIG,IK_MAX_BIG);
}


int pid_e_old = 0;
long int pid_i = 0;

void loop(){

  filter_0.addData(getIK(A0));
  filter_1.addData(getIK(A1));
  filter_2.addData(getIK(A2));

  Serial.print(filter_0.getData());
  Serial.print(" ");
  Serial.print(filter_1.getData());
  Serial.print(" ");
  Serial.print(filter_2.getData());
  Serial.println();

  if (filter_1.getData()>10) {
    int motor_speed = map(filter_1.getData(),IK_MIN,IK_MAX,MOTOR_SPEED/2,MOTOR_SPEED); // 30
    float e = filter_2.getData() - filter_0.getData();
    // float e = filter_2.getData() - 60;
    float pid_p = e;
    pid_i = pid_i*0.95+e;
    float pid_d = e - pid_e_old;
    pid_e_old = e;
    float pid = pid_p*0.3 + pid_i*0 + pid_d*0; // 20: 0.3
    // Serial.println(pid);
    MotorShield.run(motor_speed+pid,motor_speed-pid);
  }
  else {
    MotorShield.run(-30,-30);
    delay(500);
    if (pid_i>0) MotorShield.run(30,-30);
    else MotorShield.run(-30,30);
    delay(500);
  }
}

