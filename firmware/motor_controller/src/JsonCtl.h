/****************************************************************************
 * Copyright (c) 2021 by Focus Robotics. All rights reserved.
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
 * Creation_Date:  Sat Jan 16 2021
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

#ifndef JSONCTL__H
#define JSONCTL__H

#include "Arduino.h"
#include "RPBase.h"

/*typedef struct {
  byte joyx;
  byte joyy;
  byte buttons;
  } jsctl_info;*/

class JsonCtl {
 public:
  JsonCtl(Stream &s); // This can be a usbserial or hwserial class, must have begin() called on it already at the correct baud rate
  ~JsonCtl();
  void setup();
  void loop();

  // This is called from loop if I keep a ref to the serial port, or from the top of the sketch
  void process_input_char(char c);

  // These are called by process_input_char() when it receives a full command
  // They simply process cmd to avoid an extra data copy
  //void update_joystick_state();
  //void update_button_state();
  //char* get_status_string();

  // Run the test which is currently selected; start and stop as appropriate on button changes
  // called from loop, or from the top of the sketch
  //void run_test();
  void register_test(RPBase* t, uint8_t id);

  void update_ctl_struct();
  void clear_name_val_info();
  //int32_t val_string_to_num();
  //float val_string_to_float();

  Stream & ser;
  enum ParserState {NOT_IN_CMD, CMD_NAME, CMD_VAL, CMPL_VAL, CMPL_CMD, ERROR};
  ParserState jpstate; // JSON Parser State
  uint8_t name_char_count;
  uint8_t val_char_count;
  char name[8];
  char val[8];

  rpctl_info ctlinfo; // This struct always has the current joystick state

  RPBase* test[8]; // up to 8 tests can be registered to run when directed
  uint8_t current_test;

  long bt_prev_send;
  int bt_send_interval;

};
bool is_alphanumeric(char c);
bool is_numeric(char c);
#endif
