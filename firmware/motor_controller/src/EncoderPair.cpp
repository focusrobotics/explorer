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
 * - This needs to be broken into an EncoderBase and then extended to an EncoderIncremental
 *   and an EncoderQuadrature to support different encoder types that I have. You need to tell
 *   the incremental encoder the direction based on how you drive the motor, but the quadrature
 *   encoder can tell by the pulse train.
 * 
 * Limitations:
 * 
 * Testing:
 * 
 ******************************************************************************/

#include "EncoderPair.h"
#include <Arduino.h>

// FIXME should pass interrupt pins into the constructor to make this class more general
EncoderPair::EncoderPair(uint8_t rIntPin, uint8_t lIntPin) {
  rightIntPin = rIntPin;
  leftIntPin = lIntPin;
  //lastLeftCount = 0;
  //lastRightCount = 0;
  //lastLeftCountMillis = 0;
  //lastRightCountMillis = 0;
  //lastUpdateMillis = 0;
}

EncoderPair::~EncoderPair() {
}

void EncoderPair::setup() {
  // Initialize encoder pins and ISRs
  pinMode(leftIntPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(leftIntPin), leftEncoderISR, CHANGE);
  pinMode(rightIntPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(rightIntPin), rightEncoderISR, CHANGE);
}

void EncoderPair::loop() {
  // Update position and velocity and acceleration if counts have changed, or sufficient time has passed
  // Maybe only update position and heading
  
  // Have counts changed? If not, only maybe update lastUpdateTime and maybe shift lastCounts and return
  
  // Lets make some hugely simplifying assumptions to start:
  // If both motors are forwards or backwards, we are going straight
  // If one motor is forwards and the other backwards, we are turning in place
  // If one motoro is on and the other off, that is an unhandled error condition
  // I'm making these assumptions because the higher level code only operates in those modes
  // Thus, this encoder class will have a generic API, but a specific implementaion
  
  // If counts have changed, or sufficient time has passed, do an update
  // Maybe only update if both have changed or it has been at least 50ms
  // If both changed, but one more than the other, maybe use the min change to calc position
  // Should the Locomotion class try to keep counts the same?
  //  if(((leftCount != lastLeftCount) && (rightCount != lastRightCount)) || (millis() > lastUpdateMillis + 50)) {
    // position and perhaps heading have changed
    // perhaps only update if some minimum time has passed. updating position for every count update might introduce errors
    
    // case 1: moving forward, or motors off but was last moving forward
    //       Don't change heading, distance based on right wheel distance
    // case 2: moving backward, or motors off byt was last moving backward
    //       Don't change heading, distance based on right wheel distance
    // case 3: rotating clockwise
    //       Don't change distance. Heading based on right wheel travel. 44 steps is 180 degrees
    // case 4" rotating counter-clockwise
    //       Don't change distance. Heading based on right wheel travel. 44 steps is 180 degrees
  //  }
}

void EncoderPair::setMotState(motor_state_t rmot, motor_state_t lmot) {
  EncoderPair::rightMotState = rmot;
  EncoderPair::leftMotState = lmot;
}

void EncoderPair::printCounts() {
  Serial.print(rightCount);
  Serial.print(", ");
  Serial.println(leftCount);
}

int EncoderPair::getLeftCount() {
  return leftCount;
}

int EncoderPair::getRightCount() {
  return rightCount;
}

// ******************************************************************************** //
// ISR stuff
// ******************************************************************************** //

// Static variables used by the ISRs
unsigned int EncoderPair::leftCount = 0;
unsigned int EncoderPair::rightCount = 0;
//unsigned long EncoderPair::leftCountMillis = 0; // Tracking millis was to improve accuracy for velocity and acceleration in odometry. May or may not really need it.
//unsigned long EncoderPair::rightCountMillis = 0;
motor_state_t EncoderPair::rightMotState = MOT_FORWARD;
motor_state_t EncoderPair::leftMotState = MOT_FORWARD;

// Interrupt service routines for encoders
void leftEncoderISR() {
  if(EncoderPair::leftMotState == MOT_BACKWARD) {
    EncoderPair::leftCount--; // inc or dec depending on motor dir
  } else {
    EncoderPair::leftCount++; // inc or dec depending on motor dir
  }
  //EncoderPair::leftCountMillis = millis(); // get the exact millis at the interrupt time for better accuracy when computing velocity
}

void rightEncoderISR() {
  if(EncoderPair::rightMotState == MOT_BACKWARD) {
    EncoderPair::rightCount--; // inc or dec depending on motor dir
  } else {
    EncoderPair::rightCount++; // inc or dec depending on motor dir
  }
  //EncoderPair::rightCountMillis = millis(); // get the exact millis at the interrupt time for better accuracy when computing velocity
}



