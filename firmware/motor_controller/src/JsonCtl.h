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

class JsonCtl {
 public:
  JsonCtl(Stream &s); // This can be a usbserial or hwserial class, must have begin() called on it already at the correct baud rate
  ~JsonCtl();
  void setup();
  void loop();

  // The top level main routine calls this during setup to register multiple RPBase programs so the
  // robot firmware can support different behaviors or modes of operation.
  void register_test(RPBase* t, uint8_t id);

  // Internal calls, probably make private
  void process_input_char(char c);
  void update_ctl_struct();
  void clear_name_val_info();

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

};
bool is_alphanumeric(char c);
bool is_numeric(char c);
#endif
