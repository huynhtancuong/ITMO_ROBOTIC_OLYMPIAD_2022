#ifndef _LINE_H_
#define _LINE_H_

class LineSensor {
    public:
        int envValue = 0;
        int lineValue = 0;
        int pin;

        int getValueA();
        int getValueD();
        void init();

};

class LineSensors {
    public:
        LineSensor line1, line2;
        bool prevState = false;
        bool is_intersec(); // return true if 2 sensor is 1
        bool is_intersec_rising(); // only return true on rising
        void init();
        bool is00();
        bool is11();
        bool is10();
        bool is01();
};

#endif