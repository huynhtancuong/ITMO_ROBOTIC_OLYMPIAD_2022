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
    servo.write(180);
    delay(500);
    for (int i = 180; i>=170; i--) {
        servo.write(i);
        delay(50);
        if (distance_sensor.objectDetected(distance)) {
            servo.write(90);
            return true;
        }
    }
    return false;
}

bool ObjectFinder::is_object_on_right(int distance) {
    servo.write(0);
    delay(500);
    for (int i = 0; i<=10; i++) {
        servo.write(i);
        delay(50);
        if (distance_sensor.objectDetected(distance)) {
            servo.write(90);
            return true;
        }
    }
    return false;
}