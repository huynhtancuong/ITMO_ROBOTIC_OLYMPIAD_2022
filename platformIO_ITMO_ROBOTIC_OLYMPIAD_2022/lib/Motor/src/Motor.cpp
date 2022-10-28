#include "Motor.h"

void Motor::init() {
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, 0);
  analogWrite(pwmPin, 0);
  speed_ctr.I = 0;
}

void Motor::stop() {
  run(0);
}

void Motor::run_inverse_dir(int speed) {
  if (currentDirection == FORWARD) {
    run(-speed);
  }
  if (currentDirection == BACKWARD) {
    run(speed);
  }
}

double Motor::getSpeed() {
    
  Second currentTime = getTime();
  Second dt = currentTime - prevTimeGetSpeed;

  if (dt >= 0.3) {
  
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

  // Serial.print(getSpeed());
  // Serial.print(", ");
//  Serial.print(pwm);

  prevPwm = pwm;

  }

  run(prevPwm);
}

void Motor::run(int pwm) {

  if (abs(pwm) > 255) pwm = copysign(1, pwm) * 255;

  if (getTime() - prevTimeRun >= 0.0) {
    prevTimeRun = getTime();
    if (pwm >=0) // FORWARD
    {
      currentDirection = FORWARD;
      digitalWrite(dirPin, 1);
      analogWrite(pwmPin, abs(pwm));
    }
    else // BACKWARD
    {
      currentDirection = BACKWARD;
      digitalWrite(dirPin, 0);
      analogWrite(pwmPin, abs(pwm));
    }
  }
}

void Motor::run_cal(int percent) {
  if (abs(percent) > 100) percent = copysign(1, percent)*100;

  int pwm = 0;
  if (percent>0) {
    pwm = map(percent, 0, 100, 80, 255);
  }
  else if (percent < 0) {
    pwm = map(percent, -100, 0, -255, -80);
  }
  run(pwm);

}

Radian Motor::getPositionR() { // return position of motor in Radian
  return encoderCounter * ((2*PI)/encoderNumber); 
}

Degree Motor::getPositionD() { // return position of motor in Degree
  return encoderCounter * (double(360)/encoderNumber); 
}

void Motor::set_speed_control(double KP, double KI, double KD) {
  Motor::speed_ctr.KP = KP;
  Motor::speed_ctr.KI = KI;
  Motor::speed_ctr.KD = KD;
}

void Motor::set_encoderNumber(int num) {
  Motor::encoderNumber = num;
}
void Motor::set_pin(int interuptPin, int pwmPin, int dirPin) {
  Motor::interuptPin = interuptPin;
  Motor::pwmPin = pwmPin;
  Motor::dirPin = dirPin;
}