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

#include "JsonCtl.h"

JsonCtl::JsonCtl(Stream &s) : ser(s) {}

JsonCtl::~JsonCtl() {}

void JsonCtl::setup() {
  // Init any class variables
  jpstate = NOT_IN_CMD;
  current_test = 0;

  // Flush the serial stream. The higher level must have already called begin at the appropriate baud rate.
  while(ser.available()) { ser.read(); }
}

void JsonCtl::loop() {
  // process any input chars that are waiting
  while(ser.available()) {
    process_input_char(ser.read());
  }

  // Nothing is sent back over the serial link from here. Lower level classes send JSON directly to the port
  // based on the debug_info and debug_freq values set from the input stream.

  // Run subprogram currently selected
  test[current_test]->loop(&ctlinfo);
}

void JsonCtl::register_test(RPBase* t, uint8_t id) {
  test[id] = t;
}

// The format is JSON
// Commands start with '{' and end with '}'
// All chars except alphanumeric, underscore, color, and comma are ignored (no whitespace or other special chars like quotes)
// I can make it more robust in the future to ignore whitespace and quotes and error on other special chars
// collect name until a colon and then value until a comma or EOM. No name or value more than 4 ascii chars (maybe more later)
// So each char is checked against {}:, and alphanumeric; there is state, name, value
// states are not_in_cmd=0, in_cmd_name=1, in_cmd_val=2, cmpl_name_val=3, cmpl_name_val_end=4
// if not_in_cmd, dump everything until { and then go to in_cmd_name
// if in_cmd_name append to name with any alphanumeric until : and then go to in_cmd_val
// if in_cmd_val append to name with any alphanumeric until , and then go cmpl_name_val or until } and then go to cmpl_name_val_end
// if in cmpl_name_val then update the structure if the name is known and go to in_cmd_name
// if in cmpl_name_val_end update the structure if the name is known and go to no_in_cmd
void JsonCtl::process_input_char(char c) {
  //ser.print("ic: ");
  //ser.println(c);
  switch(jpstate) {
  case NOT_IN_CMD:
    if(c=='{') { jpstate = CMD_NAME; }
    break;
  case CMD_NAME:
    if(is_alphanumeric(c)) { name[name_char_count++] = c; }
    else if(c==':') { jpstate = CMD_VAL; }
    // else {} do nothing for other chars for now, could detect errors if not space or maybe quotes
    break;
  case CMD_VAL:
    if(is_numeric(c)) { val[val_char_count++] = c; }
    else if(c==',') { update_ctl_struct(); clear_name_val_info(); jpstate = CMD_NAME; }
    else if(c=='}') { update_ctl_struct(); clear_name_val_info(); jpstate = NOT_IN_CMD; }
    // else {} do nothing for other chars for now, could detect errors if not space or maybe quotes
    break;
  case CMPL_VAL:
    update_ctl_struct();
    clear_name_val_info();
    jpstate = CMD_NAME;
    break;
  case CMPL_CMD:
    update_ctl_struct();
    clear_name_val_info();
    jpstate = NOT_IN_CMD;
    break;
  case ERROR:
    clear_name_val_info();
    jpstate = NOT_IN_CMD;
    break;
  }
}

// FIXME: this was an OK way to quickly get started but it has already grown too large for such a simple
// approach. There needs to be a string matching function and a table of commands instead of this overgrown
// 'if' structure.
void JsonCtl::update_ctl_struct() {
  if(name[0]=='j' && name[1]=='s' && name[2]=='x' && name[3]==0) {
    ctlinfo.jsx = atoi(val);
    ser.print("{\"info\": \"jsx set to "); ser.print(ctlinfo.jsx); ser.println("\"}");
  } else if(name[0]=='j' && name[1]=='s' && name[2]=='y' && name[3]==0) {
    ctlinfo.jsy = atoi(val);
    ser.print("{\"info\": \"jsy set to "); ser.print(ctlinfo.jsy); ser.println("\"}");
  } else if(name[0]=='b' && name[1]=='t' && name[2]=='0' && name[3]==0) {
    ctlinfo.bt0 = atoi(val);
    ser.print("{\"info\": \"bt0 set to "); ser.print(ctlinfo.bt0); ser.println("\"}");
  } else if(name[0]=='v' && name[1]=='e' && name[2]=='l' && name[3]=='r' && name[4]==0) {
    ctlinfo.velR = atof(val);
    ser.print("{\"info\": \"velr set to "); ser.print(ctlinfo.velR); ser.println("\"}");
  } else if(name[0]=='v' && name[1]=='e' && name[2]=='l' && name[3]=='l' && name[4]==0) {
    ctlinfo.velL = atof(val);
    ser.print("{\"info\": \"vell set to "); ser.print(ctlinfo.velL); ser.println("\"}");
  } else if(name[0]=='k' && name[1]=='p' && name[2]==0) {
    ctlinfo.Kp = atoi(val);
    ser.print("{\"info\": \"kp set to "); ser.print(ctlinfo.Kp); ser.println("\"}");
  } else if(name[0]=='k' && name[1]=='i' && name[2]==0) {
    ctlinfo.Ki = atoi(val);
    ser.print("{\"info\": \"ki set to "); ser.print(ctlinfo.Ki); ser.println("\"}");
  } else if(name[0]=='k' && name[1]=='d' && name[2]==0) {
    ctlinfo.Kd = atoi(val);
    ser.print("{\"info\": \"kd set to "); ser.print(ctlinfo.Kd); ser.println("\"}");
  } else if(name[0]=='d' && name[1]=='i' && name[2]=='n' && name[3]=='f' && name[4]==0) {
    ctlinfo.debug_info = atoi(val);
    ser.print("{\"info\": \"debug_info set to "); ser.print(ctlinfo.debug_info); ser.println("\"}");
  } else if(name[0]=='d' && name[1]=='f' && name[2]=='r' && name[3]=='q' && name[4]==0) {
    ctlinfo.debug_freq = atoi(val);
    ser.print("{\"info\": \"debug_freq set to "); ser.print(ctlinfo.debug_freq); ser.println("\"}");
  } else if(name[0]=='t' && name[1]=='s' && name[2]=='t' && name[3]=='n' && name[4]==0) {
    current_test = atoi(val);
    ser.print("{\"info\": \"current_test set to "); ser.print(current_test); ser.println("\"}");
  }    
    //  } else if(name[0]=='' && name[1]=='' && name[2]=='' && name[3]=='' && name[4]==0) {
}

void JsonCtl::clear_name_val_info() {
    name_char_count = 0;
    val_char_count = 0;
    for(int i=0; i<8; i++) {name[i]=0; val[i]=0;}
}

bool is_alphanumeric(char c) {
  // numeric or upper case alpha or lower case alpha or underscore
  return ((c>=0x30 && c<=0x39) || (c>=0x41 && c<=0x5a) || (c>=0x61 && c<=0x7a) || (c==0x5f));
}

bool is_numeric(char c) {
  return ((c>=0x30 && c<=0x39) || c=='+' || c=='-' || c=='.');
}
