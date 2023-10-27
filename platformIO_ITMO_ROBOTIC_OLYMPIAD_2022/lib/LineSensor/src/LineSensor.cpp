#include <LineSensor.h>
#include <Arduino.h>

void LineSensor::init() {
    pinMode(pin, INPUT);
}

void LineSensors::init() {
    left.init();
    right.init();
}


int LineSensor::getValueA() {
    return analogRead(pin);
}

int LineSensor::getValueD() {
    if (getValueA() > (lineValue)*8/10) {
        return true;
    }
    return false;
}

bool LineSensor::is_color() {
    int top_limit = lineValue*7/10;
    int bottom_limit = lineValue*5/10;
    int analog_value = getValueA();
    if ((analog_value >= bottom_limit) && (analog_value <= top_limit)) return true;
    return false;
}

bool LineSensors::is_color_intersec() {
    if (left.is_color() && right.is_color()) {
        return true;
    }
    return false;
}

bool LineSensors::is_intersec() {
    if (left.getValueD() && right.getValueD()) {
        return true;
    }
    return false;
}

bool LineSensors::is_intersec_rising() {
    if ((is_intersec() == 1) && prevState == 0) {
        prevState = true;
        return true;
    }
    prevState = is_intersec();
    return false;
}




bool LineSensors::is_state(bool leftState, bool rightState) {
    if ((left.getValueD() == leftState) && right.getValueD() == rightState) return true;
    return false;
}

void LineSensors::setRangeValue(int leftMax, int leftMin, int rightMax, int rightMin) {
    left.envValue = leftMin;
    left.lineValue = leftMax;
    right.envValue = rightMin;
    right.lineValue = rightMax;
}