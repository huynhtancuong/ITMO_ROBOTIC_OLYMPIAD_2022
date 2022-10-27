#ifndef _MAIN_H_
#define _MAIN_H_

void leftEncoderIntFunc();
void rightEncoderIntFunc();

void odometry_update();
void odometry_interupt_init();
void run_until_intersec();
void show_line_sensor_value();

void run_for_interval(Second time);

void turn_180_left();
void turn_180_right();

void turn_left();
void turn_right();
void pickup();
void drop();
void grabber_test();
void task2();


#endif