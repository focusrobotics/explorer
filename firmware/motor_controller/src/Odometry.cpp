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
 * Limitations:
 * 
 * Testing:
 * 
 ******************************************************************************/

#include "Odometry.h"
// Need Arduino.h for the millis() function to get relative times
#include <Arduino.h>
#include <math.h>

// Fetch
// Encoders are 40 ticks per rev
// Wheel diameter is 67.25mm
// Baseline is 147mm
// From that, calculate 5.2818 mm/tick

// Explorer
// Encoders are 6533.312 ticks per rev
// Wheel diameter is 98mm, circ is 307.876mm
// Baseline is 320mm
// 21.2206 ticks/mm
// .047124 mm/tick

Odometry::Odometry(Encoder* re, Encoder* le, double b, double rdt, double ldt) {
  renc = re;
  lenc = le;
  robot_baseline = b;
  right_dist_tick = rdt;
  left_dist_tick = ldt;
}

Odometry::~Odometry() {

}

void Odometry::setup() {
  pose.x = 0;
  pose.y = 0;
  pose.heading = 0;
}

void Odometry::loop() {
  calc_pose();
}

// Velocity is reported in meters/second
double Odometry::get_velocity(motor_select_t m) {
  if(m == RIGHT_MOTOR) {
    return right_velocity;
  } else {
    return left_velocity;
  }
}

// Acceleration currently isn't calculated and just returns garbage
double Odometry::get_acceleration(motor_select_t m) {
  if(m == RIGHT_MOTOR) {
    return right_acceleration;
  } else {
    return left_acceleration;
  }
}

// Pose x and y is in mm from the origin and heading is in radians
robot_pose Odometry::get_pose() {
  return pose;
}

void Odometry::set_pose(robot_pose new_pose) {
  pose = new_pose;
}

void Odometry::calc_pose() {
  // get ticks from encoders
  int curRightTicks = renc->read();
  int curLeftTicks = lenc->read();
  unsigned long curMillis = millis(); // The call to getRightCount/getLeftCount should also return millis for each for better precision

  // calc velocites and accelerations
  // How many ticks since the last time we calculated? (How many incremental ticks?)
  // How many milliseconds since the last time we calculated? (How many incremental miliseconds?)
  int rightIncrTicks = curRightTicks - prevRightTicks;
  double rightIncrDist = rightIncrTicks * right_dist_tick;
  int rightIncrMillis = curMillis - prevRightMillis;
  right_velocity = rightIncrDist/rightIncrMillis; // mm/ms is the same as m/s

  int leftIncrTicks = curLeftTicks - prevLeftTicks;
  double leftIncrDist = leftIncrTicks * left_dist_tick;
  int leftIncrMillis = curMillis - prevLeftMillis;
  left_velocity = leftIncrDist/leftIncrMillis;

  prevRightTicks = curRightTicks;
  prevLeftTicks = curLeftTicks;
  prevRightMillis = curMillis;
  prevLeftMillis = curMillis;

  // calc new position and heading
  // distance traveled is the average of the left and right wheel
  double incrDist = (rightIncrDist + leftIncrDist) / 2;
  double deltaRotRad = (rightIncrDist - leftIncrDist) / robot_baseline;
  pose.heading += deltaRotRad;
  if(pose.heading > 2.0 * PI) { pose.heading -= 2.0 * PI; }
  if(pose.heading < 0.0) { pose.heading += 2.0 * PI; }
  pose.x += incrDist * cos(pose.heading);
  pose.y += incrDist * sin(pose.heading);
}

