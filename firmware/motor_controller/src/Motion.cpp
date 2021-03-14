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
 * Creation_Date:  Mon Dec  2 2019
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

#include "Motion.h"

Motion::Motion(MotorPair* m, Odometry* o)
  : lPID(&lMotorVelocity, &lMotorPower, &lMotorRequestedVelocity, 4000, 10000, 0, DIRECT),
    rPID(&rMotorVelocity, &rMotorPower, &rMotorRequestedVelocity, 4000, 10000, 0, DIRECT)
{
  mot = m;
  odom = o;
  lMotorPower = 0;
  rMotorPower = 0;
  lMotorState = 0;
  rMotorState = 0;
}
Motion::~Motion() {}

void Motion::setup() {
  // Initialize motors
  mot->setup();
  odom->setup();
  rPID.SetOutputLimits(-1023,1023);
  lPID.SetOutputLimits(-1023,1023);
  rPID.SetMode(AUTOMATIC);
  lPID.SetMode(AUTOMATIC);
}

void Motion::loop() {    
  // Call loop() on the Odometry class to update velocity and acceleration
  // Get the computed velocities and accelerations to use here
  odom->loop();
  rMotorVelocity = odom->get_velocity(RIGHT_MOTOR);
  lMotorVelocity = odom->get_velocity(LEFT_MOTOR);
  Serial.print("rreqVel="); Serial.print(rMotorRequestedVelocity);
  Serial.print(" rvel="); Serial.print(rMotorVelocity);
  Serial.print(" lreqVel="); Serial.print(lMotorRequestedVelocity);
  Serial.print(" lvel="); Serial.print(lMotorVelocity);
  
  // Call compute() on the PID controllers to compute motor power based on actual velocity and requested velocity for each motor
  lPID.Compute();
  rPID.Compute();

  // Set the motor power values to what the PID controller computed
  rmp = rMotorPower;
  lmp = lMotorPower;
  Serial.print(" rmp="); Serial.print(rMotorPower); Serial.print(" lmp="); Serial.println(lMotorPower); 
  if(rmp != prmp) {
    mot->set_speed(RIGHT_MOTOR, rmp);
    prmp = rmp;
  }
  if(lmp != plmp) {
    mot->set_speed(LEFT_MOTOR, lmp);
    plmp = lmp;
  }
}

// When setting velocity, if the velocity is >0 for any wheel, set the motor direction forward and tell the encoder we are moving forward
// Likewise, set the direction backward and tell the encoder if velocity is negative
// This might cause problems if changing from positive to negative velocity when the wheel is moving, but we shouldn't do that
// For Fetch, slowest is probably 0.05m/s but 0.1m/s is a more reasonable slow speed
// The fastest it can achieve with no load is about 0.7m/s but probably 0.5 or 0.6 is really the fastest I can do reliably under load at 6v
void Motion::set_velocity(double v, velocity_setting_t t) {
  switch(t) {
  case RIGHT:
    rMotorRequestedVelocity = v;
    break;
  case LEFT:
    lMotorRequestedVelocity = v;
    break;
  case BOTH:
    rMotorRequestedVelocity = v;
    lMotorRequestedVelocity = v;
    break;
  case OPPOSITE:
    rMotorRequestedVelocity = v;
    lMotorRequestedVelocity = -v;
    break;
  }
}

void Motion::set_acceleration(double a) {
  max_acc = a;
}

void Motion::set_pid_constants(uint32_t Kp, uint32_t Ki, uint32_t Kd) {
  lPID.SetTunings(Kp, Ki, Kd);
  rPID.SetTunings(Kp, Ki, Kd);
}
