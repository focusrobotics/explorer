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

#include <Encoder.h>
#include "MotorPair.h"

typedef struct {
  double x;
  double y;
  double heading;
} robot_pose;

class Odometry {
 public:
  // baseline in mm, distance per tick in mm/tick
  // b is baseline and ldt, rdt are separate right and left distances per tick
  Odometry(Encoder* re, Encoder* le, double b, double ldt, double rdt); // pass in robot specific stuff to constructor
  ~Odometry();
  void setup();
  void loop();

  // get the current pose
  robot_pose get_pose(); // will return a pose struct
  // set the current pose (for init of pose or for adjusting pose due to localization)
  void set_pose(robot_pose new_pose); // will take a pose struct, cannot fail
  double get_velocity(motor_select_t m);
  double get_acceleration(motor_select_t m);

  // Other public functions
  void reset_encoders(); // safely clear encoder values without confusing pose or velocity
  // should there be a function to read the encoders here for when I want those values at higher levels?

  // Compute the current pose based on the previous pose and any wheel encoder changes since the last call
  // This is called from the loop() function at a limited rate
  void calc_pose();

  // Encoder classes used to measure wheel distance traveled
  Encoder* renc;
  Encoder* lenc;

  // Robot params saved from constructor and used for calcs
  double robot_baseline;
  double left_dist_tick;
  double right_dist_tick;

  // Current pose
  robot_pose pose;

  // velocity and acceleration for both wheels... needed for Motion class and PID control
  // m/s or m/s/s
  double left_velocity;
  double left_acceleration;
  double right_velocity;
  double right_acceleration;

  // Keep previous state for incremental distance and velocity calcs
  int prevRightTicks;
  int prevLeftTicks;
  unsigned long prevRightMillis;
  unsigned long prevLeftMillis;
};
#endif
