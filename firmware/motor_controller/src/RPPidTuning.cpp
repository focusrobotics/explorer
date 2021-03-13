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
}

void RPPidTuning::status() {
  Serial.print("RPPidTuning Status\n");
}

void RPPidTuning::loop(rpctl_info* ctl) {
  //Serial.print("Running rpman loop\n");

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
