#ifndef _CAR_H_
#define _CAR_H_

#include "Motor.h"
#include <Arduino.h>


class Car {
  public:
    Motor leftMotor, rightMotor;
    Coordinate coord; // coordinate of the robot
    Coordinate goal; // coordiante of the goal
    Meter wheelRadius = 0.065/2; // radius of the wheel
    Meter distanceBetweenWheels = 0.126; // distance between the wheel
    Second startTime;
    Second currentTime;
    Second prevTime;
    Radian heading = 0;
    Radian bearing = 0;
    Radian course = 0;
    Meter distance_error = 0.05;
    PID straight, rotation;
    
    
    void init();
    void run(int left, int right);
    void stop();
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
