
#include "Grabber.h"
#include <Arduino.h>

void Grabber::init(int grabPin, int upPin) {
    Grab.attach(grabPin);
    Up.attach(upPin);
    Grabber::up();
}

void Grabber::up() {
    Grab.write(45);
    delay(500);
    Up.write(120);
    delay(500);
}

void Grabber::down() {
    Up.write(20);
    delay(1000);
    Grab.write(150);
    delay(500);
}