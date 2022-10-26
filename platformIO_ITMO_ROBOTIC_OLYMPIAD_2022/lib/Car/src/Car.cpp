#include "Car.h"

void Car::init() {
  startTime = getTime();
  leftMotor.init();
  rightMotor.init();
  odometry_init();
  line.line1.pin = A0;
  line.line2.pin = A1;
  // line.line1.envValue = 500;
  // line.line1.lineValue = 845;
  // line.line2.envValue = 650;
  // line.line2.lineValue = 895;
  line.line1.envValue = 800;
  line.line1.lineValue = 961;
  line.line2.envValue = 800;
  line.line2.lineValue = 970;
  line.init();
}

void Car::run(int left, int right) {
  leftMotor.run(left);
  rightMotor.run(right);
}
void Car::stop() {
  leftMotor.run(0);
  rightMotor.run(0);
}

void Car::forward(int pwm) {
  leftMotor.run(pwm);
  rightMotor.run(pwm);
}
void Car::backward(int pwm) {
  leftMotor.run(-pwm);
  rightMotor.run(-pwm);
}
void Car::turnLeft_delay() { //150
  int pwm = 150;

  long int prevLeftEncoderCounter = leftMotor.encoderCounter;
  long int prevRightEncoderCounter = rightMotor.encoderCounter;

  leftMotor.run(-pwm);
  rightMotor.run(pwm);
  delay(800);
  stop();

  long int dLeftCounter = leftMotor.encoderCounter - prevLeftEncoderCounter;
  long int dRightCouter = rightMotor.encoderCounter - prevRightEncoderCounter;

  Serial.print(dLeftCounter);
  Serial.print(", ");
  Serial.println(dRightCouter);


}
void Car::turnRight_delay() {
  int pwm = 150;

  long int prevLeftEncoderCounter = leftMotor.encoderCounter;
  long int prevRightEncoderCounter = rightMotor.encoderCounter;

  leftMotor.run(pwm);
  rightMotor.run(-pwm);
  delay(600);

  long int dLeftCounter = leftMotor.encoderCounter - prevLeftEncoderCounter;
  long int dRightCouter = rightMotor.encoderCounter - prevRightEncoderCounter;

  Serial.print(dLeftCounter);
  Serial.print(", ");
  Serial.println(dRightCouter);
}

void Car::turnLeft_encoder(int speed) {
  PID left, right;

  left.KP = 0.1;
  left.KI = 0.03;
  right.KP = 0.1;
  right.KI = 0.2;

  left.prevTime = getTime();
  right.prevTime = getTime();

  int leftSetpoint = -15;
  int rightSetpoint = 75;

  long int prevLeftEncoderCounter = leftMotor.encoderCounter;
  long int prevRightEncoderCounter = rightMotor.encoderCounter;

  long int dLeftCounter = leftMotor.encoderCounter - prevLeftEncoderCounter;
  long int dRightCouter = rightMotor.encoderCounter - prevRightEncoderCounter;

  int leftError = leftSetpoint - dLeftCounter;
  int rightError = rightSetpoint - dRightCouter;

  Serial.print(leftError);
  Serial.print(", ");
  Serial.println(rightError);

  while ((leftError <= 0) || (rightError >=0)) {
    // update vars 
    dLeftCounter = leftMotor.encoderCounter - prevLeftEncoderCounter;
    dRightCouter = rightMotor.encoderCounter - prevRightEncoderCounter;
    leftError = leftSetpoint - dLeftCounter;
    rightError = rightSetpoint - dRightCouter;

    left.dt = getTime() - left.prevTime;
    right.dt = getTime() - right.prevTime;

    left.prevTime = getTime();
    right.prevTime = getTime();

    left.I += leftError*left.dt;
    right.I += rightError*right.dt;

    int leftPwm = left.KP * leftError + left.KI * left.I;
    int rightPwm = right.KP * rightError + right.KI * right.I;

    leftMotor.run(leftPwm);
    // rightMotor.run(rightPwm);

    Serial.print(leftError);
    Serial.print(", ");
    Serial.println(rightError);

  }

}

void Car::turnRight_encoder(int speed) {

}

void Car::odometry_init() {
  coord.x = 0;
  coord.y = 0;
  course = 0;
  straight.KP = 100;
  straight.KI = 20;
  rotation.KP = 30;
  rotation.KI = 15;
}

void Car::update_coordinate() {

  Radian curLeftAngle = leftMotor.getPositionR();
  Radian curRightAngle = rightMotor.getPositionR();
//
//  Serial.print(curLeftAngle);
//  Serial.print(", ");
//  Serial.println(curRightAngle);
//
// Serial.print(leftMotor.encoderCounter);
//  Serial.print(", ");
//  Serial.println(rightMotor.encoderCounter);

  Radian dLeftAngle = curLeftAngle - leftMotor.prevPosition;
  Radian dRightAngle = curRightAngle - rightMotor.prevPosition;

  leftMotor.prevPosition = curLeftAngle;
  rightMotor.prevPosition = curRightAngle;

  course = get_course_angle(curLeftAngle, curRightAngle);

  coord.x += cos(course) * (dRightAngle + dLeftAngle) * wheelRadius / 2;
  coord.y += sin(course) * (dRightAngle + dLeftAngle) * wheelRadius / 2;

  Meter deltaX = goal.x - coord.x;
  Meter deltaY = goal.y - coord.y;

  distance = sqrt(deltaX*deltaX + deltaY*deltaY);

  bearing = atan2(deltaY, deltaX);

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
  prevTime = currentTime;
  update_goal(x, y);

  update_coordinate();

  while (!isNearGoal(distance_error)) {
    run( get_pwm_left() , get_pwm_right() );

    // Serial.println( get_pwm_left() );
    
    Serial.print(coord.x);
    Serial.print(", ");
    Serial.print(coord.y);
    Serial.print(", ");
    Serial.print(heading);
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
  currentTime = getTime() - startTime;
  straight.dt = currentTime - straight.prevTime;
  straight.prevTime = currentTime;

  straight.I += distance*straight.dt;
  if (abs(straight.I) > 100/straight.KI) straight.I = copysign(1, straight.I) * 100/straight.KI;

   double linearSpeed = straight.KP * distance * cos(heading) + straight.KI * straight.I;
//  double linearSpeed = straight.KP * distance + PID_KI * PID_integral;
  if (abs(linearSpeed) > 150) linearSpeed = copysign(1, linearSpeed) * 150;
  return linearSpeed;
}

double Car::get_angularSpeed() {
  currentTime = getTime() - startTime;
  rotation.dt = currentTime - rotation.prevTime;
  rotation.prevTime = currentTime;

  rotation.I += heading*rotation.dt;
  if (abs(rotation.I) > 100/rotation.KI) rotation.I = copysign(1, rotation.I) * 100/rotation.KI;
  
   double angularSpeed = straight.KP * sin(heading) * cos(heading) + rotation.KP * heading;
//  double angularSpeed = rotation.KP * heading;
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
