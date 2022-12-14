#include "Motor.h"

void Motor::init() {
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, 0);
  analogWrite(pwmPin, 0);
  speed_ctr.KP = 1;
  speed_ctr.KI = 1;
  speed_ctr.I = 0;
}

double Motor::getSpeed() {
    
  Second currentTime = getTime();
  Second dt = currentTime - prevTimeGetSpeed;

  if (dt >= 0.1) {
  
    prevTimeGetSpeed = currentTime;
  
    long int dEncoderCounter = encoderCounter - prevEncoderCounter;
    prevEncoderCounter = encoderCounter;
    
    RPM speed = (60/dt)*(double(dEncoderCounter)/encoderNumber);
    prevSpeed = speed;
  }

  return prevSpeed;
}

void Motor::run_rpm(RPM rpm) {

  Second dt = getTime() - speed_ctr.prevTime;

  if (dt >= 0) {
  
  speed_ctr.prevTime = getTime();
  
  RPM speedError = rpm - getSpeed();

  speed_ctr.I += speedError*dt;
  
  int pwm = speed_ctr.KP * (speedError) + speed_ctr.KI * speed_ctr.I;

  Serial.print(getSpeed());
  Serial.print(", ");
//  Serial.print(pwm);

  prevPwm = pwm;

  }

  run(prevPwm);
}

void Motor::run(int pwm) {

  if (pwm > 255) pwm = 255;
  if (pwm < -255) pwm = -255;

  if (getTime() - prevTimeRun >= 0.0) {
    prevTimeRun = getTime();
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
}

Radian Motor::getPositionR() { // return position of motor in Radian
  return encoderCounter * ((2*PI)/encoderNumber); 
}

Degree Motor::getPositionD() { // return position of motor in Degree
  return encoderCounter * (double(360)/encoderNumber); 
}
