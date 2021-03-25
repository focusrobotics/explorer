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
 * 
 * Limitations:
 * 
 * Testing:
 * 
 ******************************************************************************/

/* 
Top level for Fetch or Explorer robots
*/
#include "Arduino.h"
#include "RPBase.h"
#include "RPManual.h"
//#include "JoystickCtl.h"
#include "JsonCtl.h"

#ifdef AFMS_MOTOR
#include "AFMS_MotorPair.h"
#endif

#ifdef VNH5019_MOTOR
#include "VNH5019_MotorPair.h"
#endif

#include "Encoder.h"
#include "Odometry.h"
#include "Motion.h"
#include "RPManualMotion.h"
#include "RPPidTuning.h"

// Define constants for the interrupt pins used by the encoder and the type of encoder
// There might also be a couple of derived encoder types like BasicEncoder and QuadratureEncoder, etc.
// My encoder class is really and EncoderPair. There is an Arduino built-in Encoder library I should look at.
// Quadrature encoders need 2 pins per channel or 4 pins for an encoder pair
//Encoder enc(3, 2);
// Switching to PJRC's Encoder library which just works for quadrature encoders and Teensy
Encoder enc1(10, 9); // encoder for M1 (left wheel)
Encoder enc2(12, 11); // encoder for M2 (right wheel)
// Odometry takes an EncoderBase and the baseline of the robot and left and right distance per encoder tick for this robot
// The current values are for Fetch and will be different for Explorer
//Odometry odom(&enc, 145, 5.4, 5.4);
// Encoder ticks per mm: 6533.312 ticks/rev pi*98mm = circumference = 307.8760800136, ticks/mm = 21.2205897896, mm/tick = .047124
Odometry odom(&enc2, &enc1, 320, 0.047124, 0.047124); // baseline measured at 320mm, wheel diameters could be different 

// MotorPair for whatever motor controller this robot has
// Fetch has AFMS_MOTOR and Explorer uses VNH5019_MOTOR
#ifdef AFMS_MOTOR
AFMS_MotorPair mot;
#endif
#ifdef VNH5019_MOTOR
// Pass in the pins that VNH5019 uses since I won't be using default for STM32 or Teensy. Need 10 pins by default.
// for each motor: dirA/inA, dirB/inB, en/diag, pwm, cs (current measurement); obviously I could skip some of those
// but I should plan on passing 10 pin numbers to the constructor.
VNH5019_MotorPair mot;
#endif

// The motion class will need to be extended based on the motor controller type. AFMS for this robot but VHN*** for explorer
// There will need to be a MotionBase class and then various extended classes for different configurations
Motion motion(&mot, &odom);
// RPManual depends on the motor controller to allow joystick control of motor power
// RPMotion would use a MotionBase
RPManual rpman(&mot);
RPManualMotion rpmm(&motion);
RPPidTuning rppid(&motion);

//JoystickCtl jsctl;
JsonCtl jsctl(Serial);
// Other RP classes would not be joystick controlled, but might be kicked off with button presses from the joystick or activated from
// other Ctl classes like RasPiCtl or SerialCtl or something like that.
//unsigned long last_odom_print;
unsigned long last_jsctl;

void setup() {
  // Initialize serial port for debug
  //Serial.begin(9600);           // set up Serial library for debug at 9600 bps
  //Serial.println("Fetch Robot starting up...");
  Serial.begin(115200);
  mot.setup();
  rpman.setup();
  rpmm.setup();
  rppid.setup();
  jsctl.setup();
  jsctl.register_test(&rpman, 0);  // register rpman with jsctl
  jsctl.register_test(&rpmm, 1);  // register rpmm with jsctl
  jsctl.register_test(&rppid, 2);  // register rppid with jsctl

  //last_odom_print = millis();
  last_jsctl = millis();
}

// Main loop
void loop() {
  if(millis() > (last_jsctl + 100)) {
    last_jsctl = millis();
    jsctl.loop();
  }
  // if(millis() > (last_odom_print + 200)) {
  //   last_odom_print = millis();

  //   //odom.loop();
  //   double rvel = odom.get_velocity(RIGHT_MOTOR);
  //   double lvel = odom.get_velocity(LEFT_MOTOR);
  //   robot_pose p = odom.get_pose();
  //   int32_t pos1 = enc1.read();
  //   int32_t pos2 = enc2.read();

  //   Serial.print("pos1="); Serial.print(pos1); Serial.print(" pos2="); Serial.print(pos2); 
  //   Serial.print(" rv="); Serial.print(rvel); Serial.print(" lv="); Serial.print(lvel);
  //   Serial.print(" x="); Serial.print(p.x); Serial.print(" y="); Serial.print(p.y); Serial.print(" heading="); Serial.println(p.heading); 
  // }

}

