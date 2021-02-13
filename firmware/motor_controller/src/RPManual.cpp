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
 * Creation_Date:  Sun Nov 24 2019
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

#include "RPManual.h"
#include <Arduino.h> // For Serial and abs

RPManual::RPManual(MotorPair *m) : mot(m) {
  lMotorPower = 0;
  rMotorPower = 0;
}

RPManual::~RPManual() {
}

void RPManual::setup() {
  // Initialize motors
  mot->set_speed(RIGHT_MOTOR, rMotorPower);
  mot->set_speed(LEFT_MOTOR, lMotorPower);
}

void RPManual::status() {
  //Serial.print("RPManual Status\n");
}

void RPManual::loop(rpctl_info* ctl) {
  //Serial.print("Running rpman loop\n");
  // MotorPair takes a speed for each motor between +/- 1023
  // The joystick control gives a value between +/- 100
  // To map motor speed onto the joystick:
  // Motor speed is initially set to 10*joyy. That gives +/- 1000 for each motor or almost full range
  // If joyx is above a threshold then it modifies this velocity to cause a turn
  // First divide motor power by ((abs(joyx)+1)/10)--that should divide by between 1 to 10 to slow things down a lot for extreme turns
  // But that significan slowdown may not be good--not sure
  // Then add joyx*2 to left and subtract it from right, I think that will allow for low speed turning
  int16_t lpow = (ctl->jsy * 10);
  lpow /= (abs(ctl->jsx)+1);
  int16_t rpow = lpow;

  lpow += ctl->jsx * 2;
  rpow -= ctl->jsx * 2;

  if(lpow != lMotorPower || rpow != rMotorPower) {
    //Serial.print("rpman jsx ");
    //Serial.print(ctl->jsx);
    //Serial.print(" ");
    //Serial.print(ctl->jsy);
    //Serial.print(" ");
    //Serial.print(lpow);
    //Serial.print(" ");
    //Serial.print(rpow);
    //Serial.println();
    lMotorPower = lpow;
    rMotorPower = rpow;
    mot->set_speed(RIGHT_MOTOR, rMotorPower);
    mot->set_speed(LEFT_MOTOR, lMotorPower);
  }
}
