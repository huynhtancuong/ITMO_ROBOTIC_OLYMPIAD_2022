#include "Car.h"

void Car::init() {
  startTime = getTime();
  leftMotor.init();
  rightMotor.init();
}

void Car::run(int left, int right) {
  leftMotor.run(left);
  rightMotor.run(right);
}

void Car::forward(int pwm) {
  leftMotor.run(pwm);
  rightMotor.run(pwm);
}
void Car::backward(int pwm) {
  leftMotor.run(-pwm);
  rightMotor.run(-pwm);
}
void Car::turnLeft(int pwm);
void Car::turnRight(int pwm);

void Car::update_coordinate() {
  currentTime = getTime() - startTime;

  Radian curLeftAngle = leftMotor.getPositionR();
  Radian curRightAngle = rightMotor.getPositionR();

  Radian dLeftAngle = curLeftAngle - leftMotor.prevPosition;
  Radian dRightAngle = curRightAngle - rightMotor.prevPosition;

  leftMotor.prevPosition = curLeftAngle;
  rightMotor.prevPosition = curRightAngle;

  Radian course = get_course_angle(curLeftAngle, curRightAngle);

  coord.x += cos(course) * (dRightAngle + dLeftAngle) * wheelRadius / 2;
  coord.y += sin(course) * (dRightAngle + dLeftAngle) * wheelRadius / 2;

  Meter deltaX = goal.x - coord.x;
  Meter deltaY = goal.y - coord.y;

  distance = sqrt(deltaX*deltaX + deltaY*deltaY);

  Radian bearing = atan2(deltaY, deltaX);

  heading = bearing - course;

  if (abs(heading) > PI) {
    heading -= copysign(1, heading) * 2 * PI;
  }

}

Radian Car::get_course_angle(Radian curLeftAngle, Radian curRightAngle) {
  return (curRightAngle - curLeftAngle) * wheelRadius / distanceBetweenWheels;
}

void Car::move_to(double x, double y) {
  currentTime = getTime() - startTime;

  update_goal(x, y);

  update_coordinate();

  while (!isNearGoal(0.025)) {
    currentTime = getTime() - startTime;
    run( get_pwm_left() , get_pwm_right() );

    // Serial.println( get_pwm_left() );
    
    Serial.print(coord.x);
    Serial.print(", ");
    Serial.print(coord.y);
    Serial.print(", ");
    Serial.println(distance);
  }  

  run(0, 0);

}

void Car::update_goal(double x, double y) {
  goal.x = x;
  goal.y = y;
}

bool Car::isNearGoal(double error) {
  update_coordinate();
  if (distance <= error) return true;
  return false;
}

double Car::get_linearSpeed() {
  // double linearSpeed = K_STRAIGHT * distance * cos(heading);
  double linearSpeed = K_STRAIGHT * distance;
  if (abs(linearSpeed) > 150) linearSpeed = copysign(1, linearSpeed) * 150;
  return linearSpeed;
}

double Car::get_angularSpeed() {
  // double angularSpeed = K_STRAIGHT * sin(heading) * cos(heading) + K_ROTATION * heading;
  double angularSpeed = K_ROTATION * heading;
  if (abs(angularSpeed) > 100) angularSpeed = copysign(1, angularSpeed) * 100;
  return angularSpeed;
}

int Car::get_pwm_right() {
  int pwmRight = get_linearSpeed() + get_angularSpeed();
  if (abs(pwmRight) > 255) pwmRight = copysign(1, pwmRight) * 100;
  return pwmRight;
}

int Car::get_pwm_left() {
  int pwmLeft = get_linearSpeed() - get_angularSpeed();
  if (abs(pwmLeft) > 255) pwmLeft = copysign(1, pwmLeft) * 100;
  return pwmLeft;
}




