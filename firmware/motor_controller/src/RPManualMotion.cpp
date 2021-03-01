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
 * Creation_Date:  Fri Jan  3 2020
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

#include "RPManualMotion.h"
#include <Arduino.h>
RPManualMotion::RPManualMotion(Motion* m) {
  mot = m;
}

RPManualMotion::~RPManualMotion() {
}

void RPManualMotion::setup() {
  mot->setup();
}

void RPManualMotion::status() {
  Serial.print("RPManualMotion Status\n");
}

// The joystick range is +/-100 in x and y
// The velocity range is +/- 0.6m/s for a straight line for Fetch or Explorer
// The allowable angular range is ???
// There should be a small dead zone in the center of the joystick, say +/-10
// The robot only goes straight or rotates with this setup, no arcs are supported
void RPManualMotion::loop(rpctl_info* ctl) {
  //Serial.print("Running rpman loop\n");
  if(ctl->jsy > 10 || ctl->jsy < -10) {
    mot->set_velocity((ctl->jsy*0.006), BOTH);
  } else if(ctl->jsx > 10 || ctl->jsy < -10) {
    mot->set_velocity((ctl->jsx*0.006), OPPOSITE);
  } else {
    mot->set_velocity(0, BOTH);
  }
  mot->loop();
  
}
