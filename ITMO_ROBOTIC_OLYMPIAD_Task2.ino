#include "Car.h"


Car car;


void leftEncoderIntFunc(){
  if (car.leftMotor.currentDirection == FORWARD) {
    car.leftMotor.encoderCounter++;
  } else { // if direction is backward
    car.leftMotor.encoderCounter--;
  }
  // Serial.println(car.leftMotor.encoderCounter);
}

void rightEncoderIntFunc(){
  if (car.rightMotor.currentDirection == FORWARD) {
    car.rightMotor.encoderCounter++;
  } else { // if direction is backward
    car.rightMotor.encoderCounter--;
  }
}

void setup() {
  // init car object
  car.leftMotor.interuptPin = 0; // D2
  car.rightMotor.interuptPin = 1; // D3
  car.leftMotor.dirPin = 7;
  car.rightMotor.dirPin = 4;
  car.leftMotor.pwmPin = 6;
  car.rightMotor.pwmPin = 5;
  car.init();
  // set up interupt
  attachInterrupt(car.leftMotor.interuptPin, leftEncoderIntFunc, CHANGE);
  attachInterrupt(car.rightMotor.interuptPin, rightEncoderIntFunc, CHANGE);
  // init Serial
  Serial.begin(9600);

  // Once time code
  // car.forward(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print(car.leftMotor.getPositionR());
  // Serial.print(", ");
  // Serial.println(car.rightMotor.getPositionR());
  car.move_to(5, 5);
}








