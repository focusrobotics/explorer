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

#ifndef ENCODER__H
#define ENCODER__H

// get motor_select_t and motor_state_t from MotorPair
#include "MotorPair.h"
//typedef enum {RIGHT_MOTOR, LEFT_MOTOR} motor_select_t;
//typedef enum {MOT_FORWARD, MOT_BACKWARD} motor_state_t;

class Encoder {
 public:
  Encoder(uint8_t rIntPin, uint8_t lIntPin);
  ~Encoder();
  void setup();
  void loop(); // doesn't do anything
  void setMotState(motor_state_t rmot, motor_state_t lmot); // only needed for non-quadrature encoders, set by Motion class
  
  void printCounts();
  int getLeftCount();
  int getRightCount();

  // Pins which the encoders are connected to
  uint8_t rightIntPin;
  uint8_t leftIntPin;

  // One problem is that I can't compute the heading only from my encoders, since they aren't quaderature
  // I can count steps, but don't know whether they are forward or backward!
  // For now I need a way to cheat and have the Locomotion class tell me what it is doing
  // My first thought is to pass two args to the loop() call with -1/0/1 for the direction of each wheel (left, right)

  // current left and right position
  // have to be static so that the ISRs can access
  // static also makes sense since a robot can only ever have one position
  static unsigned int rightCount;
  static unsigned int leftCount;
  static motor_state_t rightMotState;
  static motor_state_t leftMotState;
  static unsigned long rightCountMillis;
  static unsigned long leftCountMillis;
  
  unsigned long lastLeftCountMillis;
  unsigned long lastRightCountMillis;
  unsigned int lastLeftCount;
  unsigned int lastRightCount;
  unsigned long lastUpdateMillis;


};

void leftEncoderISR();
void rightEncoderISR();

#endif

