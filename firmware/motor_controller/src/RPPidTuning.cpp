/****************************************************************************
 * Copyright (c) 2021 by Focus Robotics
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
 * Creation_Date:  Wed Mar  3 2021
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

#include "RPPidTuning.h"
#include <Arduino.h>
RPPidTuning::RPPidTuning(Motion* m) {
  mot = m;
}

RPPidTuning::~RPPidTuning() {
}

void RPPidTuning::setup() {
  mot->setup();
  mot->set_debug_prints(0xffff, 0);
}

void RPPidTuning::status() {
  Serial.print("RPPidTuning Status\n");
}

// FIXME: This has evolved into having to keep a shadow copy of all the state as previous state
// and comparing to the current state every loop to see what has to be updated. Not great.
// Perhaps a better way for RP* classes to work would be to register callbacks which are called
// whenever a parameter is updated. It's not a huge deal for a small number of parameters like
// this, but it quickly becomes cumbersome and difficult to maintain and eventually can be a
// waste of cpu cycles.
//
// A quick hack that can be an improvement would be to just have a "changed" bit in the ctl struct
// and to update everything we care about when anything changes. Could also have a changed bit
// per field, but that can start to get to be a pain as well.
void RPPidTuning::loop(rpctl_info* ctl) {
  //Serial.print("Running RPPidTuning loop\n");

  if(ctl->velR != last_velr) {
    mot->set_velocity(ctl->velR, RIGHT);
    last_velr = ctl->velR;
  }
  if(ctl->velL != last_vell) {
    mot->set_velocity(ctl->velL, LEFT);
    last_vell = ctl->velL;
  }

  if(ctl->Kp != last_kp || ctl->Ki != last_ki || ctl->Kd != last_kd) {
    mot->set_pid_constants(ctl->Kp, ctl->Ki, ctl->Kd);
    last_kp = ctl->Kp;
    last_ki = ctl->Ki;
    last_kd = ctl->Kd;
  }

  mot->loop();
  
}
