#include <LineSensor.h>
#include <Arduino.h>

void LineSensor::init() {
    pinMode(pin, INPUT);
}


int LineSensor::getValueA() {
    return analogRead(pin);
}

int LineSensor::getValueD() {
    if (getValueA() > (envValue + lineValue)/2) {
        return true;
    }
    return false;
}

bool LineSensors::is_intersec() {
    if (line1.getValueD() && line2.getValueD()) {
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
    line1.init();
    line2.init();
}