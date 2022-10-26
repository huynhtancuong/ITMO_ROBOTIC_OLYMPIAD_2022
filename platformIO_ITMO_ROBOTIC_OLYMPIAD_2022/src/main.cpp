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

  // Once time code
   
  // car.leftMotor.run(140);
  car.rightMotor.run(255);
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
// car.leftMotor.run_rpm(0);
// car.rightMotor.run_rpm(0);
// Serial.println();
// Serial.print(car.leftMotor.getSpeed());
// Serial.print(", ");
// Serial.println(car.rightMotor.getSpeed());

  // car.move_to(1, 1);

  // static long int prevEncoderCounter = 0;
  // if (prevEncoderCounter != car.leftMotor.encoderCounter) {
  //   prevEncoderCounter = car.leftMotor.encoderCounter;
  //   Serial.print(millis());
  //   Serial.print(": ");
  //   Serial.println(car.leftMotor.encoderCounter);
  // }


  // static long int prevEncoderCounter = 0;
  // if ((millis() >= 10000) && (millis() <=10100)) {
  //   if (prevEncoderCounter != car.rightMotor.encoderCounter) {
  //     prevEncoderCounter = car.rightMotor.encoderCounter;
  //     Serial.print(millis());
  //     Serial.print(": ");
  //     Serial.println(car.rightMotor.encoderCounter);
  //   }
  // }
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