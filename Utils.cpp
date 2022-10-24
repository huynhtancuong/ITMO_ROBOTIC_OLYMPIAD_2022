#include "Utils.h"
#include <Arduino.h>

Second getTime() {
  return double(millis())/1000;
}