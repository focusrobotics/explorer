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

#ifndef JOYSTICKCTL__H
#define JOYSTICKCTL__H

#include "RPBase.h"
#include "Odometry.h"

/*typedef struct {
  byte joyx;
  byte joyy;
  byte buttons;
  } jsctl_info;*/

class JoystickCtl {
 public:
  JoystickCtl();
  ~JoystickCtl();
  void setup();
  void loop();

  // This is called from loop if I keep a ref to the serial port, or from the top of the sketch
  void process_input_char(char c);

  // These are called by process_input_char() when it receives a full command
  // They simply process cmd to avoid an extra data copy
  void update_joystick_state();
  void update_button_state();
  char* get_status_string();

  // Run the test which is currently selected; start and stop as appropriate on button changes
  // called from loop, or from the top of the sketch
  void run_test();
  void register_test(RPBase* t);
  void register_odometer(Odometry* o);

  char cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // command received from bluetooth
  unsigned char cmd_state; // could be an enum. Could also just count 0 to 7

  rpctl_info jsinfo; // This struct always has the current joystick state

  RPBase* test; // up to 6 tests can be registered to run when buttons are pressed
  Odometry* odom; // just keep this temporarily for testing

  long bt_prev_send;
  int bt_send_interval;

  const char SOH = 0x01;
  const char STX = 0x02;
  const char ETX = 0x03;
  const char EOT = 0x04;
  const char ENQ = 0x05;
};
#endif
