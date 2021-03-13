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
  bt_send_interval = 750;

  // Flush the serial stream. The higher level must have already called begin at the appropriate baud rate.
  //ser.begin(9600);
  while(ser.available()) { ser.read(); }
}

void JsonCtl::loop() {
  // process any bluetooth input chars that are waiting
  while(ser.available()) {
    process_input_char(ser.read());
  }

  // send the status string back to bluetooth if it has been long enough
  long current_millis = millis();
  if(current_millis - bt_prev_send > bt_send_interval) {
    bt_prev_send = current_millis;

    // Status will be sent in JSON format and fields will be sent when requested.
  }

  // Run subprogram if any are selected; just run manual at first
  //Serial.print(tests);
  //test->status();
  test->loop(&ctlinfo);
}

void JsonCtl::register_test(RPBase* t) {
  test = t;
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

void JsonCtl::update_ctl_struct() {
  if(name[0]=='j' && name[1]=='s' && name[2]=='x' && name[3]==0) {
    ctlinfo.jsx = atoi(val);
    ser.print("{info: 'jsx set to "); ser.print(ctlinfo.jsx); ser.println("'}");
  } else if(name[0]=='j' && name[1]=='s' && name[2]=='y' && name[3]==0) {
    ctlinfo.jsy = atoi(val);
    ser.print("{info: 'jsy set to "); ser.print(ctlinfo.jsy); ser.println("'}");
  } else if(name[0]=='b' && name[1]=='t' && name[2]=='0' && name[3]==0) {
    ctlinfo.bt0 = atoi(val);
    ser.print("{info: 'bt0 set to "); ser.print(ctlinfo.bt0); ser.println("'}");
  } else if(name[0]=='v' && name[1]=='e' && name[2]=='l' && name[3]=='r' && name[4]==0) {
    ctlinfo.velR = atof(val);
    ser.print("{info: 'velr set to "); ser.print(ctlinfo.velR); ser.println("'}");
  } else if(name[0]=='v' && name[1]=='e' && name[2]=='l' && name[3]=='l' && name[4]==0) {
    ctlinfo.velL = atof(val);
    ser.print("{info: 'vell set to "); ser.print(ctlinfo.velL); ser.println("'}");
  } else if(name[0]=='k' && name[1]=='p' && name[2]==0) {
    ctlinfo.Kp = atoi(val);
    ser.print("{info: 'kp set to "); ser.print(ctlinfo.Kp); ser.println("'}");
  } else if(name[0]=='k' && name[1]=='i' && name[2]==0) {
    ctlinfo.Ki = atoi(val);
    ser.print("{info: 'ki set to "); ser.print(ctlinfo.Ki); ser.println("'}");
  } else if(name[0]=='k' && name[1]=='d' && name[2]==0) {
    ctlinfo.Kd = atoi(val);
    ser.print("{info: 'kd set to "); ser.print(ctlinfo.Kd); ser.println("'}");
  }
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
/*
// string is ascii numbers up to 3 chars with + or - optionally first
// so can return between -999 and +999 decimal
// since this can be 1 or 2 or 3 or 4 ascii chars, need to reorg the data first
// loop from the end to find the first non-null char and that is the 1's place to save it in the accum
// I actually have the number of chars so I could skip searching for nulls and just start processing at val[val_char_count-1]
// FIXME: make this generic by passing in the array pointer and char count
int32_t JsonCtl::val_string_to_num() {
  int32_t place_value_mult = 1;
  int32_t accum_val = 0;
  //ser.print("tonum ");
  //ser.print(val_char_count);
  for(int i=val_char_count; i>0; i--) {
    //ser.print("tonum ");
    //ser.print(i);
    //ser.print(" ");
    //ser.println(val[i-1]);
    if(val[i-1] == '+') { continue; }
    else if(val[i-1] == '-') { accum_val = accum_val * -1; }
    else if(val[i-1] >= 48 && val[i-1] <= 57) {
      accum_val += (val[i-1] - 48) * place_value_mult;
      place_value_mult *= 10;
      //ser.print("to_num char ");
      //ser.print(val[i-1]);
      //ser.print(" acc ");
      //ser.println(accum_val);
    } // else it's an error, but how do I report it?
  }
  return accum_val;
}

// string is ascii numbers up to n chars with initial + or - or a . somewhere in the value, all optional
// first, break off anything to the right of the decimal point, if one exists
// then, convert everything to the left of the decimal point with val_string_to_num()
// then, convert everything to the right of the decimal point with a slightly different algo where place value is divided by 10 each char
float JsonCtl::val_string_to_float() {
  // scan for a decimal point and proceed depending on whether it exists and where
  //
  int dec_count = -1;
  for(int i=0; i<val_char_count; i++) {
    if(val[i]=='.') {
      dec_count = i;
      break;
    }
  }

  if(dec_count==-1) { // no decimal point found
    return val_string_to_num();
  } else { // decimal point at dec_count
    // get the whole number part
    // get the fractional number part
    // get the sign
  }
  
  return -999.999;
}
*/
