#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <stdint.h>
#include <Arduino.h>
#include "Utils.h"

enum Direction {FORWARD, BACKWARD};

class Motor {
  
  public:
    int interuptPin; // pin for interupt
    int pwmPin; // pin for PWM
    int dirPin; // pin for direction control
    int encoderNumber = 50;
    volatile long int encoderCounter = 0;
    enum Direction currentDirection; // current direction of the motor 
    long int prevEncoderCounter = 0;
    PID speed_ctr;
    Radian prevPosition = 0;
    RPM prevSpeed = 0;
    Second prevTimeGetSpeed = 0;
    Second prevTimeRun = 0;
    int prevPwm = 0;
    
    void init();
    void run(int speed);
    void run_rpm(RPM rpm);
    Radian getPositionR(); // return position of motor in radian
    Degree getPositionD(); // return position of motor in Degrees
    RPM getSpeed(); // return rotation per minute
};

#endif
