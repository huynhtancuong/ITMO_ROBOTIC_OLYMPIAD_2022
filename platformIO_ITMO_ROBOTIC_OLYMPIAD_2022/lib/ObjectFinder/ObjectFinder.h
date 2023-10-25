#ifndef _OBJFS_H_
#define _OBJFS_H_

#include <Servo.h>
#include "Ultrasonic.h"

class ObjectFinder {
    public:
        int pin;
        Servo servo;
        Ultrasonic distance_sensor;
        void init(Ultrasonic distance_sensor);
        void set_pin(int pin);
        bool is_object_on_left(int distance);
        bool is_object_on_right(int distance);

};

#endif _OBJFS_H_