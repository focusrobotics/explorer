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

#ifndef VNH5019_MOTORPAIR__H
#define VNH5019_MOTORPAIR__H
#include <MotorPair.h>
//#include <DualVNH5019MotorShield.h>

class VNH5019_MotorPair: public MotorPair {
 public:
  VNH5019_MotorPair();
  virtual ~VNH5019_MotorPair();
  virtual void setup();
  virtual void set_speed(motor_select_t mot, int16_t speed);

  //DualVNH5019MotorShield md;
 private:
  uint8_t _INA1;
  uint8_t _INB1;
  uint8_t _PWM1;
  uint8_t _EN1DIAG1;
  uint8_t _CS1;
  uint8_t _INA2;
  uint8_t _INB2;
  uint8_t _PWM2;
  uint8_t _EN2DIAG2;
  uint8_t _CS2;

};

#endif
