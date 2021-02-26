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

//Motion::Motion(MotorPair* m, Encoder* e, Odometry* o)
Motion::Motion(MotorPair* m, Odometry* o)
  : lPID(&lMotorVelocity, &lMotorPower, &lMotorRequestedVelocity, 2, 5, 1, DIRECT),
    rPID(&rMotorVelocity, &rMotorPower, &rMotorRequestedVelocity, 2, 5, 1, DIRECT)
{
  mot = m;
  //enc = e;
  odom = o;
  //rMotor = AFMS.getMotor(1);
  //lMotor = AFMS.getMotor(2);
  lMotorPower = 100;
  rMotorPower = 108;
  lMotorState = 0;
  rMotorState = 0;
}
Motion::~Motion() {}

void Motion::setup() {
  // Initialize motors
  //AFMS.begin();  // create with the default frequency 1.6KHz
  //rMotor->run(RELEASE);
  //lMotor->run(RELEASE);
  //rMotor->setSpeed(rMotorPower);
  //lMotor->setSpeed(lMotorPower);
  mot->setup();
  //enc->setup();
  odom->setup();
}

void Motion::loop() {    
  // Call loop() on the Encoder class to update position and velocity, etc.
  // Not necessary... enc just counts ticks and is driven by interrupts
  //enc->loop();

  // Call loop() on the Odometry class to update velocity and acceleration
  // Get the computed velocities and accelerations to use here
  odom->loop();
  rMotorVelocity = odom->get_velocity(RIGHT_MOTOR);
  lMotorVelocity = odom->get_velocity(LEFT_MOTOR);
  
  // Call compute() on the PID controllers to compute motor power based on actual velocity and requested velocity for each motor
  lPID.Compute();
  rPID.Compute();

  //rMotor->setSpeed(rMotorPower);
  //lMotor->setSpeed(lMotorPower);

  mot->set_speed(RIGHT_MOTOR, rMotorPower);
  mot->set_speed(LEFT_MOTOR, lMotorPower);
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
    //enc.setMotState(); // FIXME or should this be called in loop() where we set motor speed? Need to be able to set motor direction individually
    break;
  case LEFT:
    lMotorRequestedVelocity = v;
    //enc.setMotState(); // FIXME
    break;
  case BOTH:
    rMotorRequestedVelocity = v;
    lMotorRequestedVelocity = v;
    //enc.setMotState(); // FIXME
    break;
  case OPPOSITE:
    rMotorRequestedVelocity = v;
    lMotorRequestedVelocity = -v;
    //enc.setMotState(); // FIXME
    break;
  }
}

void Motion::set_acceleration(double a) {
  max_acc = a;
}
