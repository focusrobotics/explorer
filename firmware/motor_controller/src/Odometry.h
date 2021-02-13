/****************************************************************************
 * Copyright (c) 2019 by Focus Robotics. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Created By   :  Andrew Worcester
 * Creation_Date:  Sat Nov 23 2019
 * 
 * Brief Description:
 * 
 * Functionality:
 * 
 * Issues:
 *
 * Encoder class
 * Should I try to do all math using ints? Or use float/doubles?
 * Floats and doubles are the same on AVR
 * An integer multiple is about 1us, float/double is 9us
 * Float/double divide is around 34us
 * So, using floats, this class might take 100's of us every 10 or so ms (and could be every 1-200ms probably since PID is likely to be every 200ms)
 * using ints, it might only take 10's of us every 10 or so ms
 * I'm not sure it matters that much
 * The PID class takes doubles, so it might be better to use floating point to avoid conversion
 * If I use units of mm/sec and mm/sec/sec, I think I'd get plenty of accuracy from integer arithmetic
 * Limitations:
 * 
 * Testing:
 * 
 ******************************************************************************/

#ifndef ODOMETRY__H
#define ODOMETRY__H

#include "Encoder.h"

typedef struct {
  double x;
  double y;
  double heading;
} robot_pose;

class Odometry {
 public:
  // baseline in mm, distance per tick in mm/tick
  Odometry(Encoder* e, double b, double ldt, double rdt); // pass in robot specific stuff to constructor
  ~Odometry();
  void setup();
  void loop();

  // Compute the current pose based on the previous pose and any wheel encoder changes since the last call
  // This is called from the loop() function, possibly at a limited rate
  // maybe just call this calc, it also needs to calc velocity and acceleration, unless I decide to only calc those on demand or something
  void calc_pose();

  // get the current pose
  robot_pose get_pose(); // will return a pose struct
  //double get_x(); // not sure if I'll keep individual get functions, but maybe easier for now
  //double get_y();
  //double get_heading();
  double get_velocity(motor_select_t m);
  double get_acceleration(motor_select_t m);

  // set the current pose (for init of pose or for adjusting pose due to localization)
  void set_pose(robot_pose new_pose); // will take a pose struct, cannot fail

  // Needs an encoder class to get ticks, specify to robot
  Encoder* enc;

  // Need other robot params
  double robot_baseline;
  double left_dist_tick;
  double right_dist_tick;

  // Needs to keep a current pose
  robot_pose pose;

  // velocity and acceleration for both wheels... needed for Motion class and PID control
  // m/s or m/s/s
  double left_velocity;
  double left_acceleration;
  double right_velocity;
  double right_acceleration;

  int prevRightTicks;
  int prevLeftTicks;
  unsigned long prevRightMillis;
  unsigned long prevLeftMillis;
};
#endif
