#ifndef _UTILS_H_
#define _UTILS_H_


typedef double Radian;
typedef long int Degree;
typedef double Meter;
typedef double Second;
typedef double RPM;

typedef struct Coordinate {
  double x = 0;
  double y = 0;
} Coordinate;

class PID {
  public:
    double KP, KI, KD;
    double I;
    Second dt;
    Second prevTime;
};

class Buzzer {
  public:
    int pin;
    void init();
    void on();
    void off();
    void tick();
};

Coordinate xx_to_XY(double x, double y, double a, double b);
Coordinate XY_to_xy(double X, double Y, double a, double b);



Second getTime();

#endif
