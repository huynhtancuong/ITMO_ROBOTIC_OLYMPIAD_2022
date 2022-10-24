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
    volatile long int encoderCounter = 0;
    enum Direction currentDirection; // current direction of the motor 
    Radian prevPosition = 0;
    
    void init();
    void run(int speed);
    Radian getPositionR(); // return position of motor in radian
    Degree getPositionD(); // return position of motor in Degrees
};

#endif