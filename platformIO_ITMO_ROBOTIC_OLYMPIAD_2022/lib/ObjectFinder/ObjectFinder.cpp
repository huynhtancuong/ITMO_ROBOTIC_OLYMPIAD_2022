#include "ObjectFinder.h"
#include <Arduino.h>

void ObjectFinder::init(Ultrasonic distance_sensor)
{
    this->distance_sensor = distance_sensor;
    servo.attach(pin);
    servo.write(90);
}

void ObjectFinder::set_pin(int pin) {
    this->pin=pin;
}

bool ObjectFinder::is_object_on_left(int distance) {
    for (int i = 135; i<=180; i++) {
        servo.write(i);
        delay(20);
        if (distance_sensor.objectDetected(distance)) {
            servo.write(90);
            return true;
        }
    }
    return false;
}

bool ObjectFinder::is_object_on_right(int distance) {
    for (int i = 45; i>=0; i--) {
        servo.write(i);
        delay(20);
        if (distance_sensor.objectDetected(distance)) {
            servo.write(90);
            return true;
        }
    }
    return false;
}