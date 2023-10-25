#include "Utils.h"
#include <Arduino.h>

Second getTime() {
  return double(millis())/1000;
}

void Buzzer::on() {
  tone(pin, 1000);
}

void Buzzer::off() {
  noTone(pin);
}

void Buzzer::init() {
  pinMode(pin, OUTPUT);
}

void Buzzer::tick() {
  on();
  delay(10);
  off();
}


Coordinate xx_to_XY(double x, double y, double a, double b) {
  double X = x - a;
  double Y = y - b;
  Coordinate XY;
  XY.x = X;
  XY.y = Y;
  return XY;
}

Coordinate XY_to_xy(double X, double Y, double a, double b) {
  double x = X + a;
  double y = Y + b;
  Coordinate xy;
  xy.x = x;
  xy.y = y;
  return xy;
}
