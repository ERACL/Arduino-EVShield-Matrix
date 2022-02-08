//doc matrix
//https://robopedia.kad-office.com/w/images/c/c9/MATRIX-Controller-Specification-v1.2.pdf

#define Robot_Matrix_H

#include "EVs_Matrix.h"



// class for robots driven by a Arduino-EVShield-Matrix system (parent class : EVs_Matrix)
class Robot_Matrix : public EVs_Matrix
{
public:

  // set the address of each motor (enumerable SH_Matrix_Motor defined in the EVs_Matrix class) 
  SH_Matrix_Motor motor_left;
  SH_Matrix_Motor motor_right;

  // bool describing which way the motor has to rotate for the robot to go forward (true: inverted, false: default)
  bool is_motor_left_inverted;
  bool is_motor_right_inverted;

  // int between 0 and 100, can be between -100 and 100 when using motors power mode
  int speed;

  // number of increments seen by the motors for 1 meter travelled by the robot (set exprimentally)
  long inc_per_meter;

  // distance between the two motor-driven wheels ('voie' en FR) in millimetres
  float track;



  // constructor
  Robot_Matrix(uint8_t i2c_address);

  // set all necessary variables
  void config(
    SH_Matrix_Motor motor_left_,
    SH_Matrix_Motor motor_right_,
    bool is_motor_left_inverted_, 
    bool is_motor_right_inverted_, 
    long inc_per_meter_,
    float track_
  );

  // reset both motors
  void reset_motors();

  // stop both motors and set a braking mode
  void stop();

  // set the speed of the motors for next action
  void set_speed(int speed_);

  // have the robot going straight and travelling the distance in millimeters (if 'distance' positive : forward ; else : backward)
  void go(float distance);

  // have the robot turning round (the center of rotation is the middle between the driving wheels). angle in degrees, trigo rotation if positive else clockwise 
  void turn(float angle);

  // have the robot going to specified relative coordonates in milimeters (x: pointing forward the robot ; y: pointing to the left of the robot) using the two previous functions : first turn() then go()
  void go_to(float x, float y);
};