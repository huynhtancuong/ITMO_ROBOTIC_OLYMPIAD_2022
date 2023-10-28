#include "Car.h"

void Car::init()
{
  startTime = getTime();
  leftMotor.init();
  rightMotor.init();
  line.init();
  object_finder.init(ultrasonic);
}

void Car::run(int left, int right)
{
  leftMotor.run(left);
  rightMotor.run(right);
}
void Car::stop()
{
  leftMotor.run(0);
  rightMotor.run(0);
}

void Car::forward(int pwm)
{
  leftMotor.run(pwm);
  rightMotor.run(pwm);
}
void Car::backward(int pwm)
{
  leftMotor.run(-pwm);
  rightMotor.run(-pwm);
}
void Car::turn_left_for_interval(unsigned int time, int pwm)
{ 
  leftMotor.run(-pwm);
  rightMotor.run(pwm);
  delay(time);
  stop();
}

void Car::rotate_left(int pwm) {
  leftMotor.run(-pwm);
  rightMotor.run(pwm);
}

void Car::rotate_right(int pwm) {
  leftMotor.run(pwm);
  rightMotor.run(-pwm);
}

void Car::rotate_left_until_state(bool left_state, bool right_state, int pwm) {
  while (!line.is_state(left_state, right_state)) {
    rotate_left(pwm);
    // add delay here
    delay(20);
    stop();
    delay(20);
  }
  stop();
}

void Car::rotate_right_until_state(bool left_state, bool right_state, int pwm) {
  while (!line.is_state(left_state, right_state)) {
    rotate_right(pwm);
    // add delay here
    delay(20);
    stop();
    delay(20);
  }
  stop();
}



void Car::turn_right_for_interval(unsigned int time, int pwm)
{
  leftMotor.run(pwm);
  rightMotor.run(-pwm);
  delay(time);
  stop();
}



void Car::run_follow_line(int pwm) {

  // int angular_speed = 0;
  int pwmLeft = pwm, pwmRight = pwm;

  if (line.is_state(0, 0)) {
    // angular_speed = 0;

  }
  else if (line.is_state(1, 1)) {
    // angular_speed = 0;
  }
  else if (line.is_state(1, 0)) { // right > left
    // angular_speed = 50; // angular > 0
    pwmLeft = -pwm;
    pwmRight = pwm;
  }
  else if (line.is_state(0, 1)) { // left > right
    // angular_speed = -50; // angular < 0
    pwmLeft = pwm;
    pwmRight = -pwm;
  }


  run(pwmLeft, pwmRight);
  delay(10);
  stop();
  delay(10);
}

void Car::run_back_follow_line(int pwm) {

  // int angular_speed = 0;
  int pwmLeft = -pwm, pwmRight = -pwm;

  if (line.is_state(0, 0)) {
    // angular_speed = 0;

  }
  else if (line.is_state(1, 1)) {
    // angular_speed = 0;
  }
  else if (line.is_state(1, 0)) {
    pwmLeft = pwm;
    pwmRight = -pwm;
  }
  else if (line.is_state(0, 1)) {
    pwmLeft = -pwm;
    pwmRight = pwm;
  }


  run(pwmLeft, pwmRight);
  delay(10);
  stop();
  delay(10);
}

void Car::stop_now(int pwm) {
  leftMotor.run(-pwm);
  rightMotor.run(-pwm);
  delay(40);
  stop();
}


void Car::pickup() {
  Car::grabber.up();
}

void Car::up() {
  Car::grabber.up_without_obj();
}

void Car::drop() {
  Car::grabber.down();
}