#include "Ultrasonic.h"
#include <Arduino.h>

void Ultrasonic::init(int trigPin, int echoPin) {
    Ultrasonic::trigPin = trigPin;
    Ultrasonic::echoPin = echoPin;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

int Ultrasonic::getDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034/2;
    return distance;
}
bool Ultrasonic::objectDetected() {
    if (getDistance() < 5) return true;
    return false;
}