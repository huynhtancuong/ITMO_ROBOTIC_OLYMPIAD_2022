#ifndef _GRABBER_H_
#define _GRABBER_H_

#include <Servo.h>

class Grabber {
    private:
        Servo Grab, Up;
    public:
        void init(int grabPin, int upPin);
        void up();
        void down();
        void up_without_obj();
};


#endif