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

#ifndef RPBASE__H
#define RPBASE__H

#include <stdint.h>

typedef struct {
  uint8_t pgm; // program select, only for the controller
  int8_t jsx;  // joystick x axis +/-127
  int8_t jsy;  // joystick y axis +/-127
  uint8_t bt0; // 8 button values
  // how do I represent velocities? float or fixed point value?
  // one digit of whole number plus 3 digits of fraction would always be more than enough, so 16 bit fixed would work
  // 16 bit values could be velocity or motor power; should I reuse the same entries or have both?
  float velR;
  float velL;
  uint32_t Kp; // PID constants for PID tuning
  uint32_t Ki;
  uint32_t Kd;
  // I want some controls to turn messages on and off and control debug and info level
} rpctl_info;

class RPBase {
 public:
  RPBase();
  virtual ~RPBase();
  virtual void setup();
  virtual void status();
  virtual void loop(rpctl_info* ctl);
};
#endif
