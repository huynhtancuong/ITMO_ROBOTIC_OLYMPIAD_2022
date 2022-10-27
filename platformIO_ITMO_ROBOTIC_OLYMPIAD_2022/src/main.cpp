#include "Car.h"
#include "main.h"
#include <TimerOne.h>

Car car;
Buzzer buzzer;


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
  odometry_interupt_init();
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

void odometry_interupt_init() {
  attachInterrupt(car.leftMotor.interuptPin, leftEncoderIntFunc, RISING);
  attachInterrupt(car.rightMotor.interuptPin, rightEncoderIntFunc, RISING);
  Timer1.initialize(80000); // 80ms
  Timer1.attachInterrupt(odometry_update);
}


void odometry_update() {
  car.update_coordinate();
}


void run_until_intersec() {
  car.forward(100); // TODO: replace with run_follow_line(speed);
  while (car.line.is_intersec_rising() == 0);
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

