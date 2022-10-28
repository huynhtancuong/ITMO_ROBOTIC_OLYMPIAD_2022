#include "Car.h"

void Car::init()
{
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
  // line.line1.envValue = 800;
  // line.line1.lineValue = 961;
  // line.line2.envValue = 800;
  // line.line2.lineValue = 970;
  // line.line1.envValue = 585;
  // line.line1.lineValue = 920;
  // line.line2.envValue = 400;
  // line.line2.lineValue = 900;
  line.line1.envValue = 780;
  line.line1.lineValue = 960;
  line.line2.envValue = 730;
  line.line2.lineValue = 950;
  line.init();
  straight.KP = 100;
  straight.KI = 20;
  rotation.KP = 30;
  rotation.KI = 15;
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
void Car::turnLeft_delay(unsigned int time)
{ // 150
  int pwm = 150;

  // long int prevLeftEncoderCounter = leftMotor.encoderCounter;
  // long int prevRightEncoderCounter = rightMotor.encoderCounter;

  leftMotor.run(-pwm);
  rightMotor.run(pwm);
  delay(time);
  stop();

  // long int dLeftCounter = leftMotor.encoderCounter - prevLeftEncoderCounter;
  // long int dRightCouter = rightMotor.encoderCounter - prevRightEncoderCounter;

  // Serial.print(dLeftCounter);
  // Serial.print(", ");
  // Serial.println(dRightCouter);
}

void Car::rotate_left_until_line_detected(int pwm) {
  while (!line.is10()) {
    leftMotor.run(-pwm);
    rightMotor.run(pwm);
  }
  stop();
}

void Car::rotate_right_until_line_detected(int pwm) {
  while (line.is00()) {
    leftMotor.run(pwm);
    rightMotor.run(-pwm);
  }
  stop();
}

void Car::turnRight_delay(unsigned int time)
{
  int pwm = 150;

  // long int prevLeftEncoderCounter = leftMotor.encoderCounter;
  // long int prevRightEncoderCounter = rightMotor.encoderCounter;

  leftMotor.run(pwm);
  rightMotor.run(-pwm);
  delay(time);
  stop();

  // long int dLeftCounter = leftMotor.encoderCounter - prevLeftEncoderCounter;
  // long int dRightCouter = rightMotor.encoderCounter - prevRightEncoderCounter;

  // Serial.print((dLeftCounter));
  // Serial.print(", ");
  // Serial.println(dRightCouter);
}

void Car::turnLeft_encoder(int speed) // suck
{
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

  while ((leftError <= 0) || (rightError >= 0))
  {
    // update vars
    dLeftCounter = leftMotor.encoderCounter - prevLeftEncoderCounter;
    dRightCouter = rightMotor.encoderCounter - prevRightEncoderCounter;
    leftError = leftSetpoint - dLeftCounter;
    rightError = rightSetpoint - dRightCouter;

    left.dt = getTime() - left.prevTime;
    right.dt = getTime() - right.prevTime;

    left.prevTime = getTime();
    right.prevTime = getTime();

    left.I += leftError * left.dt;
    right.I += rightError * right.dt;

    int leftPwm = left.KP * leftError + left.KI * left.I;
    int rightPwm = right.KP * rightError + right.KI * right.I;

    leftMotor.run(leftPwm);
    // rightMotor.run(rightPwm);

    Serial.print(leftError);
    Serial.print(", ");
    Serial.println(rightError);
  }
}

void Car::turnRight_encoder(int speed) // suck
{
}

void Car::odometry_init()
{
  coord.x = 0;
  coord.y = 0;
  course = 0;
}

void Car::update_coordinate()
{

  Radian curLeftAngle = leftMotor.getPositionR();
  Radian curRightAngle = rightMotor.getPositionR();

  Radian dLeftAngle = curLeftAngle - leftMotor.prevPosition;
  Radian dRightAngle = curRightAngle - rightMotor.prevPosition;

  leftMotor.prevPosition = curLeftAngle;
  rightMotor.prevPosition = curRightAngle;

  course = get_course_angle(curLeftAngle, curRightAngle);

  coord.x += cos(course) * (dRightAngle + dLeftAngle) * wheelRadius / 2;
  coord.y += sin(course) * (dRightAngle + dLeftAngle) * wheelRadius / 2;

  Meter deltaX = goal.x - coord.x;
  Meter deltaY = goal.y - coord.y;

  distance = sqrt(deltaX * deltaX + deltaY * deltaY);

  bearing = atan2(deltaY, deltaX);

  heading = bearing - course;

  if (abs(heading) > PI)
  {
    heading -= copysign(1, heading) * 2 * PI;
  }
}

Radian Car::get_course_angle(Radian curLeftAngle, Radian curRightAngle)
{
  return (curRightAngle - curLeftAngle) * wheelRadius / distanceBetweenWheels;
}

void Car::move_to(double x, double y)
{
  currentTime = getTime() - startTime;
  prevTime = currentTime;
  update_goal(x, y);

  update_coordinate();

  while (!isNearGoal(Car::distance_error))
  {
    double linearSpeed = get_linearSpeed();
    double angularSpeed = get_angularSpeed();
    run(get_pwm_left(linearSpeed, angularSpeed), get_pwm_right(linearSpeed, angularSpeed));

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

void Car::update_goal(double x, double y)
{
  goal.x = x;
  goal.y = y;
}

bool Car::isNearGoal(double error)
{
  update_coordinate();
  if (distance <= error)
    return true;
  return false;
}

double Car::get_linearSpeed()
{
  currentTime = getTime() - startTime;
  straight.dt = currentTime - straight.prevTime;
  straight.prevTime = currentTime;

  straight.I += distance * straight.dt;
  if (abs(straight.I) > 100 / straight.KI)
    straight.I = copysign(1, straight.I) * 100 / straight.KI;

  // choose linear or nonlinear model
  double linearSpeed = straight.KP * distance * cos(heading) + straight.KI * straight.I;
  //  double linearSpeed = straight.KP * distance + PID_KI * PID_integral;

  if (abs(linearSpeed) > 150)
    linearSpeed = copysign(1, linearSpeed) * 150;
  return linearSpeed;
}

double Car::get_angularSpeed()
{
  currentTime = getTime() - startTime;
  rotation.dt = currentTime - rotation.prevTime;
  rotation.prevTime = currentTime;

  rotation.I += heading * rotation.dt;
  if (abs(rotation.I) > 100 / rotation.KI)
    rotation.I = copysign(1, rotation.I) * 100 / rotation.KI;

  // choose linear or nonlinear model
  double angularSpeed = straight.KP * sin(heading) * cos(heading) + rotation.KP * heading;
  //  double angularSpeed = rotation.KP * heading;

  if (abs(angularSpeed) > 100)
    angularSpeed = copysign(1, angularSpeed) * 100;
  return angularSpeed;
}

int Car::get_pwm_right(double linearSpeed, double angularSpeed)
{
  int pwmRight = linearSpeed + angularSpeed;
  if (abs(pwmRight) > 255)
    pwmRight = copysign(1, pwmRight) * 100;
  return pwmRight;
}

int Car::get_pwm_left(double linearSpeed, double angularSpeed)
{
  int pwmLeft = linearSpeed - angularSpeed;
  if (abs(pwmLeft) > 255)
    pwmLeft = copysign(1, pwmLeft) * 100;
  return pwmLeft;
}


void Car::run_follow_line(int linear_speed) {

  // int angular_speed = 0;
  int pwmLeft = linear_speed, pwmRight = linear_speed;

  if (line.is00()) {
    // angular_speed = 0;

  }
  else if (line.is11()) {
    // angular_speed = 0;
  }
  else if (line.is10()) { // right > left
    // angular_speed = 50; // angular > 0
    pwmLeft   = 0;
    pwmRight  = linear_speed;
  }
  else if (line.is01()) { // left > right
    // angular_speed = -50; // angular < 0
    pwmLeft   = linear_speed;
    pwmRight  = 0;
  }

  // pwmLeft = get_pwm_left(linear_speed, angular_speed);
  // pwmRight = get_pwm_right(linear_speed, angular_speed);

  run(pwmLeft, pwmRight);
}

void Car::stop_now() {
  leftMotor.run(-50);
  rightMotor.run(-50);
  delay(50);
  stop();
}


void Car::pickup() {
  Car::grabber.up();
}

void Car::drop() {
  Car::grabber.down();
}