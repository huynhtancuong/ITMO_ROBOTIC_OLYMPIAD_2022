#include "Motor.h"

void Motor::init() {
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, 0);
  analogWrite(pwmPin, 0);
}

void Motor::run(int pwm) {

  if (pwm > 255) pwm = 255;
  if (pwm < -255) pwm = -255;

  if (pwm >=0) // FORWARD
  {
    currentDirection = FORWARD;
    digitalWrite(dirPin, 1);
    analogWrite(pwmPin, pwm);
  }
  else // BACKWARD
  {
    currentDirection = BACKWARD;
    digitalWrite(dirPin, 0);
    analogWrite(pwmPin, pwm);
  }
}

Radian Motor::getPositionR() { // return position of motor in Radian
  return encoderCounter * (PI/12); 
}

Degree Motor::getPositionD() { // return position of motor in Degree
  return encoderCounter * 15; 
}
