#include "Car.h"

Car car;
Buzzer buzzer;


void leftEncoderIntFunc(){
  noInterrupts();
  if (car.leftMotor.currentDirection == FORWARD) {
    ++car.leftMotor.encoderCounter;
  } else { // if direction is backward
    --car.leftMotor.encoderCounter;
  }
  interrupts();
  // Serial.println(car.leftMotor.encoderCounter);
}

void rightEncoderIntFunc(){
  noInterrupts();
  if (car.rightMotor.currentDirection == FORWARD) {
    ++car.rightMotor.encoderCounter;
  } else { // if direction is backward
    --car.rightMotor.encoderCounter;
  }
  interrupts();
}

void setup() {
  // init buzzer object
  buzzer.pin = 9;
  buzzer.init();
  // init car object
  car.leftMotor.interuptPin = 0; // D2
  car.rightMotor.interuptPin = 1; // D3
  car.leftMotor.dirPin = 7;
  car.rightMotor.dirPin = 4;
  car.leftMotor.pwmPin = 6;
  car.rightMotor.pwmPin = 5;
  car.leftMotor.encoderNumber = 24;
  car.rightMotor.encoderNumber = 24; //120
  car.leftMotor.speed_ctr.KP = 0.2;
  car.leftMotor.speed_ctr.KI = 0.5;
  car.rightMotor.speed_ctr.KP = 0.2;
  car.rightMotor.speed_ctr.KI = 0.5;
  
  car.init();
  // set up interupt
  attachInterrupt(car.leftMotor.interuptPin, leftEncoderIntFunc, RISING);
  attachInterrupt(car.rightMotor.interuptPin, rightEncoderIntFunc, RISING);
  // init Serial
  Serial.begin(115200);

  task2();

}

void loop() {


  Serial.print(car.line.line1.getValueD());
  Serial.print(", ");
  Serial.println(car.line.line2.getValueD());



}

void task2() {
  run_until_intersec();
  delay(500);
  turn_left();
}


void perform_motor_test() {
  unsigned long prevTime = millis();
  for (int pwm = 80; pwm <= 250; pwm+=10) {
    car.rightMotor.encoderCounter = 0;
    car.leftMotor.encoderCounter = 0;
    car.run(pwm, pwm);
    while (millis() - prevTime <= 5000);
    // Serial.print(millis() - prevTime);
    // Serial.print(", ");
    Serial.print(pwm);
    Serial.print(", ");
    Serial.print(car.leftMotor.encoderCounter);
    Serial.print(", ");
    Serial.println(car.rightMotor.encoderCounter);
    prevTime = millis();
  }
}



void run_until_intersec() {
  car.forward(100);
  while (car.line.is_intersec_rising() == 0) {
    car.update_coordinate();
  }
  car.stop();
}

void turn_left() {
  car.turnLeft_delay();
  car.stop();
}

void turn_right() {
  car.turnRight_delay();
}

void pickup() {
  car.pickup();
}

void drop() {
  car.drop();
}

