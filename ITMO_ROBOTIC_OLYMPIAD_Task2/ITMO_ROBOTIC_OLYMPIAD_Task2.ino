#include "Car.h"


Car car;
Buzzer buzzer;


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
  car.init();
  // set up interupt
  attachInterrupt(car.leftMotor.interuptPin, leftEncoderIntFunc, FALLING);
  attachInterrupt(car.rightMotor.interuptPin, rightEncoderIntFunc, FALLING);
  // init Serial
  Serial.begin(115200);

  // Once time code
   
   
  // car.leftMotor.run(255);
//  car.move_to(0.4, 0);
//  buzzer.tick();
//  car.move_to(0.4, 0.4);
//  buzzer.tick();
//  car.move_to(0, 0.4);
//  buzzer.tick();
//  car.move_to(0, 0);
//  buzzer.tick();
//  car.move_to(0.4, 0);
//  buzzer.tick();
//  car.move_to(1, 1);
//  car.stop();
}

void loop() {
  // put your main code here, to run repeatedly:
//  car.update_coordinate();
//  Serial.print(car.heading);
//   Serial.print(", ");
//  Serial.print(car.course);
//   Serial.print(", ");
//   Serial.print(car.coord.x);
//   Serial.print(", ");
//   Serial.println(car.coord.y);
car.leftMotor.run_rpm(50);
car.rightMotor.run_rpm(50);
Serial.println();
//Serial.println(car.leftMotor.getSpeed());
  // car.move_to(1, 1);
}
