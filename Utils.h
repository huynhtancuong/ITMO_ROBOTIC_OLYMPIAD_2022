#ifndef _UTILS_H_
#define _UTILS_H_

typedef double Radian;
typedef long int Degree;
typedef double Meter;
typedef double Second;

typedef struct Coordinate {
  double x = 0;
  double y = 0;
} Coordinate;

Second getTime();

#endif