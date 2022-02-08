#include "Robot_Matrix.h"
#include <math.h>

#define PI 3.141592

Robot_Matrix::Robot_Matrix(uint8_t i2c_address)
: EVs_Matrix(i2c_address)
{
}

void Robot_Matrix::config(
  SH_Matrix_Motor motor_left_,
  SH_Matrix_Motor motor_right_,
  bool is_motor_left_inverted_, 
  bool is_motor_right_inverted_, 
  long inc_per_meter_, 
  float track_
  )
{
  motor_left = motor_left_;
  motor_right = motor_right_;
  is_motor_left_inverted = is_motor_left_inverted_;
  is_motor_right_inverted = is_motor_right_inverted_;

  speed = 0;
  
  inc_per_meter = inc_per_meter_; 
  track = track_; 

  set_timeout(0);
}

void Robot_Matrix::reset_motors()
{
  // trigger reset mode
  set_motor_mode(motor_left, 0b00000100);
  set_motor_mode(motor_right, 0b00000100);
  delay(50);

  // set motor speed to 0... maybe useless
  set_motor_speed(motor_left, 0);
  set_motor_speed(motor_right, 0);
}

void Robot_Matrix::stop()
{
  set_motor_speed(motor_left,0);
  set_motor_speed(motor_right,0);
  set_motor_mode(motor_left,1); //braking
  set_motor_mode(motor_right,1); //braking
}

void Robot_Matrix::set_speed(int speed_)
{
  speed = speed_;
}

// distance in millimetres ; forward if positive, backward if negative
void Robot_Matrix::go(float distance)
{
  bool direction; // true if forward ; false if backward
  direction = distance >= 0;
  long inc; // distance to travel converted in increments
  inc = inc_per_meter * fabs(distance)/1000;

  // reset motors
  reset_motors();

  // set the parameters of both motors
  set_motor_speed(motor_left, speed);
  set_motor_speed(motor_right, speed);
  set_motor_mode(motor_left, direction!=is_motor_left_inverted ? 0b00001011 : 0b00011011);
  set_motor_mode(motor_right, direction!=is_motor_right_inverted ? 0b00001011 : 0b00011011);
  set_motor_target(motor_left, inc);
  set_motor_target(motor_right, inc);

  // start motors
  start_flag();
}

// angle in degrees ; trigo direction if positive, clockwise if negative
void Robot_Matrix::turn(float angle)
{
  bool direction; // true if trigo (angle positive) ; false if clockwise (angle negative)
  direction = angle >= 0;
  long inc; // distance to travel by each wheel (in oppsite directions) converted in increments 
  inc = inc_per_meter * track/1000 * fabs(angle)/360 * PI; 
  //inc = [inc_per_meter * distance] with distance = [2*PI/360 * angle * track/2]

  // reset motors
  reset_motors();

  // set the parameters of both motors
  set_motor_speed(motor_left, speed);
  set_motor_speed(motor_right, speed);
  set_motor_mode(motor_left, direction!=is_motor_left_inverted ? 0b00011011 : 0b00001011);
  set_motor_mode(motor_right, direction!=is_motor_right_inverted ? 0b00001011 : 0b00011011);
  set_motor_target(motor_left, inc);
  set_motor_target(motor_right, inc);

  // start motors
  start_flag();
}

// x: pointing forward ; y: pointing left
void Robot_Matrix::go_to(float x, float y)
{
  // determine polar coordonates from (x,y) to be able to use turn() and go()
  float angle = atan(y/x) * 360/(2*PI);
  if (x < 0) {
    if (y < 0) {
      angle = angle - 180;
    }
    else {
      angle = angle + 180;
    }
  }
  float distance = sqrt(x*x + y*y);

  // turning round first
  turn(angle);
  delay(100);

  // waiting for the motors to stop
  while (is_motor_busy(motor_left)==1 || is_motor_busy(motor_right)==1) {}
  delay(200);

  // then go straight (always forward)
  go(distance);
}