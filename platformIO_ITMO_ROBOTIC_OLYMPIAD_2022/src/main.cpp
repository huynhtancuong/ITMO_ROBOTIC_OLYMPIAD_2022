#include "Car.h"
#include "main.h"
#include <TimerOne.h>
#include <SharpIR.h>

void object_finder_test();
void prepare_for_turn(int pwm, int num);
void test2();
void run_until_color_intersec(int pwm);
void turn_left_not_prepare(int);
void turn_right_not_prepare(int);

int start_object_position = 0;
int finish_object_position = 2;

Car car;
Buzzer buzzer;
// SharpIR SharpIR(SharpIR::GP2Y0A21YK0F, A3);


void setup() {
  // init car object
  car.leftMotor.set_pin(0, 6, 7);
  car.rightMotor.set_pin(1, 5, 4);
  car.grabber.init(10, 11); // grabPin = 10, upPin = 11
  car.ultrasonic.init(9, 8); // trigPin = 9, echoPin = 8
  car.line.left.pin = A0;
  car.line.right.pin = A1;
  car.line.setRangeValue(966, 77, 996, 102);
  car.object_finder.set_pin(3);
  car.init();
  // Init buzzer
  buzzer.pin = 2;
  buzzer.init();
  // init Serial
  Serial.begin(115200);

  // Wait for the button
  while(analogRead(A3) < 50);

  /**
   * Begin the run
  */
  test2();
  // calibrate_line_sensor();
  // object_finder_test();
  // car.grabber.down();
  // delay(2000);
  // car.grabber.up();
}

void loop() {
  // show_line_sensor_value();
  // delay(500);
  // distance_sensor_test();
  // Serial.println(analogRead(A3));
  // delay(500);
}

void object_finder_test() {
  Serial.println(car.object_finder.is_object_on_left(40));
  delay(500);
}

void distance_sensor_test() {
  Serial.println(car.ultrasonic.getDistance());
  delay(500);
  // Serial.println(SharpIR.getDistance());
}

void grabber_test() {
  car.grabber.down();
  delay(1000);
  car.grabber.up();
  delay(1000);
}

void test2() {
  int runSpeed = 180;
  int turnSpeed = 200;

  int run_speed_to_return_to_main_line = 220;


  // This is for drop the bottle
  float run_time_to_target = 0.55; // seconds
  int run_speed_to_target = 200;



  run_until_intersec(runSpeed); // go to first intersec
  delay(500);

  /**
   * Find the bottle
  */

  run_until_intersec(runSpeed); // go to second intersec
  delay(500);

  if (car.object_finder.is_object_on_right(40)) {
    turn_right(turnSpeed);
    start_object_position = 4;
  } else if (car.object_finder.is_object_on_left(40)) {
    turn_left(turnSpeed);
    start_object_position = 1;
  } else {
    car.object_finder.servo.write(90);
    run_until_intersec(runSpeed); // go to third intersec
    delay(500);
    if (car.object_finder.is_object_on_right(40)) {
      turn_right(turnSpeed);
      start_object_position = 5;
    } else if (car.object_finder.is_object_on_left(40)) {
      turn_left(turnSpeed);
      start_object_position = 2;
    } else {
      car.object_finder.servo.write(90);
      run_until_intersec(runSpeed); // go to fourth intersec
      delay(500);
      if (car.object_finder.is_object_on_right(40)) {
        turn_right(turnSpeed);
        start_object_position = 6;
      } else {
        turn_left(turnSpeed);
        start_object_position = 3;
      }
    }
  }


 /**
  * Begin of pickup the bottle
  * After pickup, the car rotate 180* and go the the intersection
 */

  run_until_object_detected(runSpeed);
  delay(500);

  pickup();
  delay(500);

  run_forward_for_interval(0.2, runSpeed);

  turn_180_right(turnSpeed);

  run_until_intersec(run_speed_to_return_to_main_line);

  /**
   * Begin of go to the target position
  */

  if (start_object_position == 1) {
    switch (finish_object_position)
    {
    case 4:
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 2: 
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 5:
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 3: 
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    default:
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    }
  }

  if (start_object_position == 2) {
    switch (finish_object_position)
    {
    case 5:
      // run_forward_for_interval(run_time_to_object, run_speed_to_object);
      // run_back_until_intersec(runSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 1: 
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 4:
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 3: 
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    default:
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    }
  }

  if (start_object_position == 3) {
    switch (finish_object_position)
    {
    case 6:
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 1: 
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 4:
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 2: 
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    default:
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    }
  }
  
  if (start_object_position == 4) {
    switch (finish_object_position)
    {
    case 1:
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 2: 
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 5:
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 3: 
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    default:
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    }
  }

  if (start_object_position == 5) {
    switch (finish_object_position)
    {
    case 2:
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 1: 
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 4:
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 3: 
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    default:
      turn_right_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    }
  }

  if (start_object_position == 6) {
    switch (finish_object_position)
    {
    case 3:
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 1: 
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 4:
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    case 2: 
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_right_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    default:
      turn_left_not_prepare(turnSpeed);
      run_until_intersec(runSpeed);
      turn_left_not_prepare(turnSpeed);
      run_forward_for_interval(run_time_to_target, run_speed_to_target);
      drop();
      break;
    }
  }

  /**
   * Came 
   * Go back to home
  */

  // car.up();

  // turn_180_right(turnSpeed);

  // run_until_intersec(run_speed_to_return_to_main_line);

  // switch (finish_object_position)
  // {
  // case 1:
  //   turn_right(turnSpeed);
  //   run_until_intersec(runSpeed);
  //   break;
  // case 2:
  //   turn_right(turnSpeed);
  //   run_until_intersec(runSpeed);
  //   run_until_intersec(runSpeed);
  //   break;
  // case 3:
  //   turn_right(turnSpeed);
  //   run_until_intersec(runSpeed);
  //   run_until_intersec(runSpeed);
  //   run_until_intersec(runSpeed);
  //   break;
  // case 4:
  //   turn_left(turnSpeed);
  //   run_until_intersec(runSpeed);
  //   break;
  // case 5:
  //   turn_left(turnSpeed);
  //   run_until_intersec(runSpeed);
  //   run_until_intersec(runSpeed);
  //   break;
  // default:
  //   turn_left(turnSpeed);
  //   run_until_intersec(runSpeed);
  //   run_until_intersec(runSpeed);
  //   run_until_intersec(runSpeed);
  //   break;
  // }

  // car.forward(200);
  // delay(250);
  car.stop();
}


void show_line_sensor_value() {
  Serial.print(car.line.left.getValueA());
  Serial.print(", ");
  Serial.println(car.line.right.getValueA());
}

void calibrate_line_sensor() {
  int line1_max = 0;
  int line1_min = 1024;
  int line2_max = 0;
  int line2_min = 1024;
  double startTime = getTime();
  Serial.println("Calibrating Line Sensor...");
  while (getTime() - startTime <= 5) {
    int line1_value = car.line.left.getValueA();
    int line2_value = car.line.right.getValueA();
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



void run_until_intersec(int speed) {
  while (car.line.is_intersec_rising() == 0) {
    car.run_follow_line(speed); // 150
  }
  // delay(200);
  buzzer.tick();
  car.stop_now(speed);

  // while (car.line.is_intersec() == 0) {
  //   car.run_follow_line(speed); // 150
  // }
}

void run_until_color_intersec(int speed) {
  while (car.line.is_color_intersec() == 0) {
    car.run_follow_line(speed);
  }
  buzzer.tick();
  car.stop_now(speed);
}

void run_back_until_intersec(int speed) {
  while (car.line.is_intersec_rising() == 0) {
    car.backward(speed);
  }
  car.stop_now(speed);
}

void run_forward_for_interval(Second time, int pwm) {
  // car.forward(pwm);
  // delay(time);
  // Serial.println("Begin go foward");
  Second startTime = getTime();
  while ((getTime() - startTime) < time) {
    car.run_follow_line(pwm);
  }
  car.stop_now(pwm);
  // Serial.println("Stoped");

}

void run_backward_for_interval(Second time, int pwm) {
  car.backward(pwm);
  delay(time);
  car.stop_now(pwm);
}

void turn_180_right(int pwm) {
  turn_right(pwm);
}

void turn_180_left(int pwm) {
  turn_left(pwm);
}

void turn_left(int pwm) {

  prepare_for_turn(pwm, 3);

  car.rotate_left_until_state(0, 1, pwm);
  car.stop();
  car.rotate_left_until_state(0, 0, pwm);
  car.stop();
  delay(50);
  car.rotate_left_until_state(1, 0, pwm);
  car.stop();
  car.rotate_left_until_state(0, 0, pwm);
  car.stop();
}

void turn_left_not_prepare(int pwm) {

  car.rotate_left_until_state(0, 1, pwm);
  car.stop();
  car.rotate_left_until_state(0, 0, pwm);
  car.stop();
  delay(50);
  car.rotate_left_until_state(1, 0, pwm);
  car.stop();
  car.rotate_left_until_state(0, 0, pwm);
  car.stop();
}

void turn_right(int pwm) {
  
  prepare_for_turn(pwm, 3);
  
  car.rotate_right_until_state(1, 0, pwm);
  car.stop();
  car.rotate_right_until_state(0, 0, pwm);
  car.stop();
  delay(50);
  car.rotate_right_until_state(0, 1, pwm);
  car.stop();
  car.rotate_right_until_state(0, 0, pwm);
  car.stop();
}

void turn_right_not_prepare(int pwm) {
  
  car.rotate_right_until_state(1, 0, pwm);
  car.stop();
  car.rotate_right_until_state(0, 0, pwm);
  car.stop();
  delay(50);
  car.rotate_right_until_state(0, 1, pwm);
  car.stop();
  car.rotate_right_until_state(0, 0, pwm);
  car.stop();
}

void prepare_for_turn(int pwm, int num) {
  run_forward_for_interval(0.1, pwm);
}

void pickup() {
  car.pickup();
}

void drop() {
  // car.backward(100);
  // delay(500);
  car.stop();
  car.drop();
}

void run_until_object_detected(int speed) {
  while (car.ultrasonic.objectDetected(15) == false) {
    car.run_follow_line(speed); // 150
  }
  // car.run(speed, speed);
  // delay(50);
  car.stop();
}