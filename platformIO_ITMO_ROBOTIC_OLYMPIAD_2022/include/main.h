#ifndef _MAIN_H_
#define _MAIN_H_

void leftEncoderIntFunc();
void rightEncoderIntFunc();

void odometry_update();
void odometry_interupt_init();
void run_until_intersec(int speed);
void run_until_object_detected(int speed);
void show_line_sensor_value();

void run_for_interval(Second time);

void turn_180_left();
void turn_180_right();

void turn_left(int pwm);
void turn_right(int pwm);
void pickup();
void drop();
void grabber_test();
void distance_sensor_test();
void calibrate_line_sensor();
void task2();


#endif