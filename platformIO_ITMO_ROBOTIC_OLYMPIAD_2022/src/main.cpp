#include "Car.h"
#include "main.h"
#include <TimerOne.h>
#include <SharpIR.h>

Car car;
Buzzer buzzer;
SharpIR SharpIR(SharpIR::GP2Y0A21YK0F, A3);


void setup() {
  // init buzzer object
  buzzer.pin = 9;
  buzzer.init();
  // init car object
  car.leftMotor.set_pin(0, 6, 7);
  car.rightMotor.set_pin(1, 5, 4);
  car.leftMotor.set_encoderNumber(24);
  car.rightMotor.set_encoderNumber(24);
  car.leftMotor.set_speed_control(0.2, 0.5, 0);
  car.rightMotor.set_speed_control(0.2, 0.5, 0);
  car.grabber.init(11, 10);
  // car.ultrasonic.init(9, 8);
  
  car.init();
  // set up interupt
  // odometry_interupt_init();
  // init Serial
  Serial.begin(115200);

  // task2();
  // grabber_test();
  car.grabber.down();

}

void loop() {
  // Serial.print(car.line.line1.getValueD());
  // Serial.print(", ");
  // Serial.println(car.line.line2.getValueD());
  // show_line_sensor_value();
  // grabber_test();
  distance_sensor_test();
}

void distance_sensor_test() {
  // Serial.println(car.ultrasonic.getDistance());
  Serial.println(SharpIR.getDistance());
}

void grabber_test() {
  car.grabber.up();
  delay(1000);
  car.grabber.down();
  delay(1000);
}

void task2() {
  run_until_intersec();
  delay(500);

  run_until_intersec();
  delay(500);

  run_until_intersec();
  delay(500);

  turn_right();
  delay(500);

  run_for_interval(1);
  delay(500);

  turn_180_left();
  delay(500);

  run_until_intersec();
  delay(500);

  turn_left();
  delay(500);

  run_until_intersec();
  delay(500);

  run_until_intersec();
  delay(500);

  turn_right();
  delay(500);

  run_for_interval(1);


}


void leftEncoderIntFunc(){
  noInterrupts();
  if (car.leftMotor.currentDirection == FORWARD) {
    ++car.leftMotor.encoderCounter;
  } else { // if direction is backward
    --car.leftMotor.encoderCounter;
  }
  interrupts();
  // Serial.println(car.leftMotor.encoderCounter);
}

void rightEncoderIntFunc(){
  noInterrupts();
  if (car.rightMotor.currentDirection == FORWARD) {
    ++car.rightMotor.encoderCounter;
  } else { // if direction is backward
    --car.rightMotor.encoderCounter;
  }
  interrupts();
}


void show_line_sensor_value() {
  Serial.print(car.line.line1.getValueA());
  Serial.print(", ");
  Serial.println(car.line.line2.getValueA());
}


void perform_motor_test() {
  unsigned long prevTime = millis();
  for (int pwm = 80; pwm <= 250; pwm+=10) {
    car.rightMotor.encoderCounter = 0;
    car.leftMotor.encoderCounter = 0;
    car.run(pwm, pwm);
    while (millis() - prevTime <= 5000);
    // Serial.print(millis() - prevTime);
    // Serial.print(", ");
    Serial.print(pwm);
    Serial.print(", ");
    Serial.print(car.leftMotor.encoderCounter);
    Serial.print(", ");
    Serial.println(car.rightMotor.encoderCounter);
    prevTime = millis();
  }
}

void odometry_interupt_init() {
  attachInterrupt(car.leftMotor.interuptPin, leftEncoderIntFunc, RISING);
  attachInterrupt(car.rightMotor.interuptPin, rightEncoderIntFunc, RISING);
  Timer1.initialize(80000); // 80ms
  Timer1.attachInterrupt(odometry_update);
}


void odometry_update() {
  car.update_coordinate();
}


void run_until_intersec() {
  // car.forward(100); // TODO: replace with run_follow_line(speed);
  while (car.line.is_intersec_rising() == 0) {
    car.run_follow_line(150); // 150
  }
  car.stop_now();
}

void run_for_interval(Second time) {
  Second startTime = getTime();
  while (getTime() - startTime < time) {
    car.run_follow_line(150);
  }
  car.stop_now();

}

void turn_180_right() {
  turn_right();
}

void turn_180_left() {
  turn_left();
}

void turn_left() {
  car.turnLeft_delay(500);
  car.rotate_left_until_line_detected(100);
  car.stop();
}

void turn_right() {
  car.turnRight_delay(500);
  car.rotate_right_until_line_detected(100);
  car.stop();
}

void pickup() {
  car.pickup();
}

void drop() {
  car.drop();
}

