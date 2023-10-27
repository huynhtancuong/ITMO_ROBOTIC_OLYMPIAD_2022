#ifndef _LINE_H_
#define _LINE_H_

class LineSensor {
    public:
        int envValue = 0;
        int lineValue = 0;
        int pin;

        int getValueA();
        int getValueD();
        bool is_color();
        void init();

};

class LineSensors {
    public:
        LineSensor left, right;
        bool prevState = false;
        void init();
        bool is_intersec(); // return true if 2 sensor is 1
        bool is_intersec_rising(); // only return true on rising
        bool is_color_intersec();
        bool is_state(bool left, bool right);
        void setRangeValue(int leftMax, int leftMin, int rightMax, int rightMin);
};

#endif