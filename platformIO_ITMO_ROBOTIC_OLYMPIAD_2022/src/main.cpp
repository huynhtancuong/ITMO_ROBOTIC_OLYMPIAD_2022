#include "Car.h"
#include "main.h"
#include <TimerOne.h>
#include <SharpIR.h>

Car car;
// Buzzer buzzer;
// SharpIR SharpIR(SharpIR::GP2Y0A21YK0F, A3);


void setup() {
  // init buzzer object
  // buzzer.pin = 9;
  // buzzer.init();
  // init car object
  car.leftMotor.set_pin(0, 6, 7);
  car.rightMotor.set_pin(1, 5, 4);
  // car.leftMotor.set_encoderNumber(24);
  // car.rightMotor.set_encoderNumber(24);
  // car.leftMotor.set_speed_control(0.2, 0.5, 0);
  // car.rightMotor.set_speed_control(0.2, 0.5, 0);
  car.grabber.init(10, 11); // grabPin = 10, upPin = 11
  car.ultrasonic.init(9, 8); // trigPin = 9, echoPin = 8
  car.line.setRangeValue(956, 470, 898, 254);
  car.init();
  // set up interupt
  // odometry_interupt_init();
  // init Serial
  Serial.begin(115200);

  task2();
  // calibrate_line_sensor();

}

void loop() {
  // Serial.print(car.line.line1.getValueD());
  // Serial.print(", ");
  // Serial.println(car.line.line2.getValueD());
  // show_line_sensor_value();
  // grabber_test();
  // distance_sensor_test();
  // car.leftMotor.run(255);
  // car.rightMotor.run(-255);
  // car.ultrasonic.getDistance();
}

void distance_sensor_test() {
  // Serial.println(car.ultrasonic.objectDetected(10));
  // Serial.println(SharpIR.getDistance());
}

void grabber_test() {
  car.grabber.down();
  delay(1000);
  car.grabber.up();
  delay(1000);
}

void task2() {


  int runSpeed = 90;


  // turn_right();
  // run_for_interval(1);

  run_until_intersec(runSpeed);
  delay(500);

  run_until_intersec(runSpeed);
  delay(500);

  run_until_intersec(runSpeed);
  delay(500);
  
  turn_right(150);

  run_until_object_detected(runSpeed);
  delay(500);
  pickup();

  turn_180_left();

  run_until_intersec(100);
  delay(500);

  turn_left(150);

  run_until_intersec(runSpeed);
  delay(500);

  run_until_intersec(runSpeed+10);
  delay(500);

  turn_right(150);

  run_for_interval(2);

  drop();


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

void calibrate_line_sensor() {
  int line1_max = 0;
  int line1_min = 1024;
  int line2_max = 0;
  int line2_min = 1024;
  double startTime = getTime();
  Serial.println("Calibrating Line Sensor...");
  while (getTime() - startTime <= 5) {
    int line1_value = car.line.line1.getValueA();
    int line2_value = car.line.line2.getValueA();
    if (line1_max < line1_value) line1_max = line1_value;
    if (line1_min > line1_value) line1_min = line1_value; 
    if (line2_max < line2_value) line2_max = line2_value;
    if (line2_min > line2_value) line2_min = line2_value; 
  }
  Serial.println("Calibration Completed: ");
  Serial.print("Left (max, min): ");
  Serial.print(line1_max);
  Serial.print(", ");
  Serial.println(line1_min);

  Serial.print("Right (max, min): ");
  Serial.print(line2_max);
  Serial.print(", ");
  Serial.println(line2_min);

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


void run_until_intersec(int speed) {
  while (car.line.is_intersec_rising() == 0) {
    car.run_follow_line(speed); // 150
  }
  car.stop_now();
}

void run_for_interval(Second time) {
  Second startTime = getTime();
  while (getTime() - startTime < time) {
    car.run_follow_line(100);
  }
  car.stop_now();

}

void turn_180_right() {
  turn_right(150);
}

void turn_180_left() {
  turn_left(150);
}

void turn_left(int pwm) {
  car.turnLeft_delay(300, pwm);
  car.stop();
  delay(50);
  car.rotate_left_until_line_detected(pwm); // 100
  car.stop();
}

void turn_right(int pwm) {
  car.turnRight_delay(300, pwm);
  // car.rotate_right_until_line_10(100);
  car.stop();
  // car.rotate_right_until_line_00(100);
  // car.stop();
  delay(50);
  car.rotate_right_until_line_detected(pwm); // 100
  car.stop();
}

void pickup() {
  car.pickup();
}

void drop() {
  car.drop();
}

void run_until_object_detected(int speed) {
  while (car.ultrasonic.objectDetected(5) == false) {
    car.run_follow_line(speed); // 150
  }
  car.run(speed, speed);
  delay(50);
  car.stop_now();
}