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
  delay(50);
  off();
}
