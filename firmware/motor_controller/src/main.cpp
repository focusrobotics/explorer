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


// Define constants for the interrupt pins used by the encoder and the type of encoder
// There might also be a couple of derived encoder types like BasicEncoder and QuadratureEncoder, etc.
// My encoder class is really and EncoderPair. There is an Arduino built-in Encoder library I should look at.
// Quadrature encoders need 2 pins per channel or 4 pins for an encoder pair
//Encoder enc(3, 2);
// Odometry takes an EncoderBase and the baseline of the robot and left and right distance per encoder tick for this robot
// The current values are for Fetch and will be different for Explorer
//Odometry odom(&enc, 145, 5.4, 5.4);

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
//Motion motion(&mot, &enc, &odom);
// RPManual depends on the motor controller to allow joystick control of motor power
// RPMotion would use a MotionBase
RPManual rpman(&mot);
//JoystickCtl jsctl;
JsonCtl jsctl(Serial);
// Other RP classes would not be joystick controlled, but might be kicked off with button presses from the joystick or activated from
// other Ctl classes like RasPiCtl or SerialCtl or something like that.
unsigned long last_millis;

void setup() {
  // Initialize serial port for debug
  //Serial.begin(9600);           // set up Serial library for debug at 9600 bps
  //Serial.println("Fetch Robot starting up...");
  Serial.begin(115200);
  mot.setup();
  rpman.setup();
  jsctl.setup();
  jsctl.register_test(&rpman);  // register rpman with jsctl

  last_millis = millis();
  //enc.setup();
}

// Main loop
void loop() {
  jsctl.loop();
  //if(millis() > (last_millis + 500)) {
  //  last_millis = millis();

  //  odom.loop();
  //  double rvel = odom.get_velocity(RIGHT_MOTOR);
  //  double lvel = odom.get_velocity(LEFT_MOTOR);
  //  Serial.print("rv="); Serial.print(rvel); Serial.print(" lv="); Serial.println(lvel);
  //  enc.printCounts();
  //}

}

