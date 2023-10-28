
#include "Grabber.h"
#include <Arduino.h>

void Grabber::init(int grabPin, int upPin) {
    Grab.attach(grabPin);
    Up.attach(upPin);
    Grabber::down();
}

void Grabber::up() {
    Grab.write(60);
    delay(500);
    Up.write(170);
    delay(500);
}

void Grabber::up_without_obj() {
    Up.write(170);
    delay(500);
}

void Grabber::down() {
    Up.write(100);
    delay(1000);
    Grab.write(80);
    delay(1000);
    Grab.write(150);
    delay(500);
}