#ifndef _CAR_H_
#define _CAR_H_

#include "Motor.h"
#include <Arduino.h>


class Car {
  public:
    Motor leftMotor, rightMotor;
    Coordinate coord; // coordinate of the robot
    Coordinate goal; // coordiante of the goal
    Meter wheelRadius = 0.065; // radius of the wheel
    Meter distanceBetweenWheels = 0.126; // distance between the wheel
    Second startTime;
    Second currentTime;
    Radian heading = 0;
    double K_STRAIGHT=60, K_ROTATION=50;
    
    void init();
    void run(int left, int right);
    void forward(int pwm);
    void backward(int pwm);
    void turnLeft(int pwm);
    void turnRight(int pwm);
    void update_coordinate();
    void move_to(double x, double y);

  private:
    Meter distance;
    Radian get_course_angle(Radian curLeftAngle, Radian curRightAngle);
    void update_goal(double x, double y);
    bool isNearGoal(double error);
    double get_linearSpeed();
    double get_angularSpeed();
    int get_pwm_right();
    int get_pwm_left();
};


#endif