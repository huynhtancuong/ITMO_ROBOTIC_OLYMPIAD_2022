#include <LineSensor.h>
#include <Arduino.h>

void LineSensor::init() {
    pinMode(pin, INPUT);
}


int LineSensor::getValueA() {
    return analogRead(pin);
}

int LineSensor::getValueD() {
    if (getValueA() > (envValue + lineValue)*2/3) {
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

void LineSensors::init() {
    left.init();
    right.init();
}

bool LineSensors::is00() {
    if (!left.getValueD() && !right.getValueD()) return true;
    return false;
}

bool LineSensors::is01() {
    if (!left.getValueD() && right.getValueD()) return true;
    return false;
}

bool LineSensors::is10() {
    if (left.getValueD() && !right.getValueD()) return true;
    return false;
}

bool LineSensors::is11() {
    if (left.getValueD() && right.getValueD()) return true;
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