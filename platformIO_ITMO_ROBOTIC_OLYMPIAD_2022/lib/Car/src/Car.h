#ifndef _CAR_H_
#define _CAR_H_

#include "Motor.h"
#include <Arduino.h>
#include "LineSensor.h"
#include "Grabber.h"
#include "Ultrasonic.h"
#include <SharpIR.h>


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
    LineSensors line;
    Grabber grabber;
    Ultrasonic ultrasonic;
    
    
    void init();
    void run(int left, int right);
    void stop();
    void stop_now(int pwm);
    void forward(int pwm);
    void backward(int pwm);
    void turnLeft_delay(unsigned int time, int pwm);
    void turnRight_delay(unsigned int time, int pwm);
    void turnLeft_encoder(int speed);
    void turnRight_encoder(int speed);
    void update_coordinate();
    void move_to(double x, double y);
    void odometry_init();
    bool intersec_check();

    void pickup();
    void drop();
    
    void run_follow_line(int linear_speed);
    void run_back_follow_line(int linear_speed);

    void rotate_left_until_line_10(int pwm);
    void rotate_left_until_line_01(int pwm);
    void rotate_left_until_line_00(int pwm);
    void rotate_right_until_line_01(int pwm);
    void rotate_right_until_line_10(int pwm);
    void rotate_right_until_line_00(int pwm);

    void rotate_left(int pwm);
    void rotate_right(int pwm);

  private:
    Meter distance;
    Radian get_course_angle(Radian curLeftAngle, Radian curRightAngle);
    void update_goal(double x, double y);
    bool isNearGoal(double error);
    double get_linearSpeed();
    double get_angularSpeed();
    int get_pwm_right(double linearSpeed, double angularSpeed);
    int get_pwm_left(double linearSpeed, double angularSpeed);
};


#endif
