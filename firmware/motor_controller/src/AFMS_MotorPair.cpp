/****************************************************************************
 * Copyright (c) 2020 by Focus Robotics. All rights reserved.
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
 * Creation_Date:  Wed Dec 30 2020
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
#ifdef AFMS_MOTOR

#include <AFMS_MotorPair.h>

AFMS_MotorPair::AFMS_MotorPair() {
  rMotor = AFMS.getMotor(1);
  lMotor = AFMS.getMotor(2);
  lMotorState = FORWARD;
  rMotorState = FORWARD;
  lMotorPower = 0;
  rMotorPower = 0;

}

AFMS_MotorPair::~AFMS_MotorPair() {}

void AFMS_MotorPair::setup() {
  // Initialize motors
  AFMS.begin();  // create with the default frequency 1.6KHz
  rMotor->run(RELEASE);
  lMotor->run(RELEASE);
  rMotor->setSpeed(rMotorPower);
  lMotor->setSpeed(lMotorPower);

}

void AFMS_MotorPair::set_speed(motor_select_t mot, int16_t speed) {
  Adafruit_DCMotor *motor;
  if(mot==RIGHT_MOTOR) {
    motor = rMotor;
  } else {
    motor = lMotor;
  }

  if(speed > 0) {
    motor->run(FORWARD);
  } else if(speed < 0) {
    motor->run(BACKWARD);
  } else {
    motor->run(RELEASE);
  }

  // Input speed range in +/-1023 but I want 0-255 for this driver since direction is computed above
  // dividing the absolute value of input speed by 4 should be correct, but cast to a uint8 to clamp the range
  motor->setSpeed((uint8_t)(abs(speed)/4));

  //if(ctl->joyy > 10) {
  //  rMotor->run(FORWARD);
  //  lMotor->run(FORWARD);
  //  rMotor->setSpeed(ctl->joyy * 2); // joystick gives -100 to +100 so this give most of the range of +/-255 for the motor controller but not all
  //  lMotor->setSpeed(ctl->joyy * 2);
  //} else if(ctl->joyy < -10) {
  //  rMotor->run(BACKWARD);
  //  lMotor->run(BACKWARD);
  //  rMotor->setSpeed(ctl->joyy * -2);
  //  lMotor->setSpeed(ctl->joyy * -2);
  //} else {
  //  if(ctl->joyx > 10) {
  //    rMotor->run(BACKWARD);
  //    lMotor->run(FORWARD);
  //    rMotor->setSpeed(ctl->joyx * 2);
  //    lMotor->setSpeed(ctl->joyx * 2);
  //  } else if(ctl->joyx < -10) {
  //    rMotor->run(FORWARD);
  //    lMotor->run(BACKWARD);
  //    rMotor->setSpeed(ctl->joyx * -2);
  //    lMotor->setSpeed(ctl->joyx * -2);
  //  } else {
  //    rMotor->run(RELEASE);
  //    lMotor->run(RELEASE);
  //  }
  //}

}

#endif
