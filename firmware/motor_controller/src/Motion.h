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

#ifndef MOTION__H
#define MOTION__H

#include "Odometry.h" // need to get velocity and acceleration
#include "MotorPair.h"
#include <PID_v1.h>

typedef enum {RIGHT, LEFT, BOTH, OPPOSITE} velocity_setting_t;

class Motion {
 public:
  Motion(MotorPair* m, Odometry* o);
  ~Motion();
  void setup();
  void loop();

  // I think that initially I can just say that velocity of the wheels is always equal
  // Equal in the same direction for forward, backward motion
  // Equal in opposite directions for rotation clockwise or counterclockwise
  // This doesn't allow arcs of movement, but is simpler in some ways
  // A PID controller will be used to keep the velocities correct
  // Once the velocity is set, regular calls to loop() will keep adjustments going
  // Use an enum with the velocity which is LEFT, RIGHT, BOTH, or OPPOSITE and default to BOTH
  void set_velocity(double v, velocity_setting_t t);
  void set_acceleration(double a); // maximum accel as robot gets up (or down) to new velocity

  // This class stores a velocity for right and left wheels, a max acceleration, and 2 PID controllers.
  // Velocity and acceleration for each wheel come from odometry so it also has an odometery class.
  // It does not try to manage absolute position or heading, those are for a higher level to deal with.
  Odometry* odom;
  double max_acc;

  // Classes and variables to access the motor and store its current state
  MotorPair* mot;
  double lMotorPower;
  double rMotorPower;
  char lMotorState;
  char rMotorState;
  uint16_t lmp, plmp, rmp, prmp;

  double lMotorVelocity;
  double lMotorRequestedVelocity;
  PID lPID;
  double rMotorVelocity;
  double rMotorRequestedVelocity;
  PID rPID;

};
#endif
