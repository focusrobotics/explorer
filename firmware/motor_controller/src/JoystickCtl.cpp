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

#include "JoystickCtl.h"
#include "Arduino.h"

JoystickCtl::JoystickCtl() {}
JoystickCtl::~JoystickCtl() {}

void JoystickCtl::setup() {
  // Init any class variables
  cmd_state = 0;
  bt_send_interval = 750;

  // Set up Serial1 to talk to bluetooth. Make the serial port configurable in the future.
  Serial1.begin(9600);
  while(Serial1.available()) { Serial1.read(); }
}

void JoystickCtl::loop() {
  // process any bluetooth input chars that are waiting
  while(Serial1.available()) {
    process_input_char(Serial1.read());
  }

  // send the status string back to bluetooth if it has been long enough
  long current_millis = millis();
  if(current_millis - bt_prev_send > bt_send_interval) {
    bt_prev_send = current_millis;

    // Data frame transmitted back from Arduino to Android device:
    // < 0X02   Buttons state   0X01   DataField#1   0x04   DataField#2   0x05   DataField#3    0x03 >  
    // < 0X02      "01011"      0X01     "120.00"    0x04     "-4500"     0x05  "Motor enabled" 0x03 >    // example

    Serial1.print((char)STX);                                             // Start of Transmission
    Serial1.print("000001");  Serial1.print((char)0x1);   // buttons status feedback
    Serial1.print("Foo");            Serial1.print((char)0x4);   // datafield #1
    Serial1.print("Bar");            Serial1.print((char)0x5);   // datafield #2
    Serial1.print("Bas");                                        // datafield #3
    Serial1.print((char)ETX);                                             // End of Transmission
  }

  // Run subprogram if any are selected; just run manual at first
  //Serial.print(tests);
  //test->status();
  test->loop(&jsinfo);
}

void JoystickCtl::process_input_char(char c) {
  if(cmd_state == 0 && c != STX) return; // comm error, transmissions must start with STX
  
  if(c > 127 || cmd_state > 7) { // comm error, limited char set and cmd length
    cmd_state = 0;
    return;
  }
  
  if(c == ETX) { // end of transmission, update state if the right number of chars sent
    if(cmd_state == 7) update_joystick_state();
    if(cmd_state == 2) update_button_state();
    cmd_state = 0;
    return;
  }
  
  cmd[cmd_state++] = c;

}

void JoystickCtl::update_joystick_state() {
  int joyX = (cmd[1]-48)*100 + (cmd[2]-48)*10 + (cmd[3]-48);       // obtain the Int from the ASCII representation
  int joyY = (cmd[4]-48)*100 + (cmd[5]-48)*10 + (cmd[6]-48);
  joyX = joyX - 200;                                                  // Offset to avoid
  joyY = joyY - 200;                                                  // transmitting negative numbers

  if(joyX<-100 || joyX>100 || joyY<-100 || joyY>100)     return;      // commmunication error
  jsinfo.jsx = joyX;
  jsinfo.jsy = joyY;
  //Serial.print("Joystick: "); Serial.print(joyX); Serial.print(" "); Serial.print(joyY); Serial.print("\n");
}

// This could work better with subprograms if button events were sent to subprograms first
void JoystickCtl::update_button_state() {
  switch (cmd[1]) {
// -----------------  BUTTON #1  -----------------------
  case 'A':
    jsinfo.bt0 |= 0x01;        // ON
    Serial.println("\n** Button_1: ON **");
    // your code...      
    //displayStatus = "LED <ON>";
    //Serial.println(displayStatus);
    //digitalWrite(ledPin, HIGH);
    break;
  case 'B':
    jsinfo.bt0 &= ~0x01;        // OFF
    Serial.println("\n** Button_1: OFF **");
    // your code...      
    //displayStatus = "LED <OFF>";
    //Serial.println(displayStatus);
    //digitalWrite(ledPin, LOW);
    break;

// -----------------  BUTTON #2  -----------------------
  case 'C':
    jsinfo.bt0 |= 0x02;        // ON
    Serial.println("\n** Button_2: ON **");
    // your code...      
    //displayStatus = "Button2 <ON>";
    //Serial.println(displayStatus);
    break;
  case 'D':
    jsinfo.bt0 &= ~0x02;        // OFF
    Serial.println("\n** Button_2: OFF **");
    // your code...      
    //displayStatus = "Button2 <OFF>";
    //Serial.println(displayStatus);
    break;

// -----------------  BUTTON #3  -----------------------
  case 'E':
    jsinfo.bt0 |= 0x04;        // ON
    Serial.println("\n** Button_3: ON **");
    // your code...      
    //displayStatus = "Motor #1 enabled"; // Demo text message
    //Serial.println(displayStatus);
    break;
  case 'F':
    jsinfo.bt0 &= ~0x04;      // OFF
    Serial.println("\n** Button_3: OFF **");
    // your code...      
    //displayStatus = "Motor #1 stopped";
    //Serial.println(displayStatus);
    break;

// -----------------  BUTTON #4  -----------------------
  case 'G':
    jsinfo.bt0 |= 0x08;       // ON
    Serial.println("\n** Button_4: ON **");
    // your code...      
    //displayStatus = "Datafield update <FAST>";
    //Serial.println(displayStatus);
    //sendInterval = FAST;
    break;
  case 'H':
    jsinfo.bt0 &= ~0x08;    // OFF
    Serial.println("\n** Button_4: OFF **");
    // your code...      
    //displayStatus = "Datafield update <SLOW>";
    //Serial.println(displayStatus);
    //sendInterval = SLOW;
    break;

// -----------------  BUTTON #5  -----------------------
  case 'I':           // configured as momentary button
    //      jsinfo.bt0 |= 0x10;        // ON
    Serial.println("\n** Button_5: ++ pushed ++ **");
    // your code...      
    //displayStatus = "Button5: <pushed>";
    break;
    //   case 'J':
    //     jsinfo.bt0 &= ~0x10;        // OFF
    //     // your code...      
    //     break;

// -----------------  BUTTON #6  -----------------------
  case 'K':
    jsinfo.bt0 |= 0x20;        // ON
    Serial.println("\n** Button_6: ON **");
    // your code...      
    //displayStatus = "Button6 <ON>"; // Demo text message
    break;
   case 'L':
     jsinfo.bt0 &= ~0x20;        // OFF
     Serial.println("\n** Button_6: OFF **");
     // your code...      
     //displayStatus = "Button6 <OFF>";
     break;
 }
// ---------------------------------------------------------------
}

char* JoystickCtl::get_status_string() {
  return 0;
}

void JoystickCtl::run_test() {

}

void JoystickCtl::register_test(RPBase* t) {
  test = t;
  Serial.print("Registered test\n");
}

void JoystickCtl::register_odometer(Odometry* o) {
  odom = o;
  Serial.print("Registered odometer\n");
}
