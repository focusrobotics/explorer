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
 * Creation_Date:  Mon Dec  2 2019
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

#include "Sonar.h"

//#define TRIGGER_PIN  32  // Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ECHO_PIN     33  // Arduino pin tied to echo pin on the ultrasonic sensor.
//#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// Sonar
// Create the Sonar control object
//NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Sonar::Sonar(int tp, int ep, int md) {}
Sonar::~Sonar() {}
void Sonar::setup() {}
void Sonar::loop() {}

/*
void SonarAvoidStuff() {
  // Sonar data is noisy! Should require a few pings in a row to something close before stopping
  unsigned int cm = sonar.ping_cm(); // Send ping, get distance in cm
  if(cm>0 && cm<30) {
    loc.turnAround();
    loops--;
    Serial.print("Found obstacle. Turning around with left and right counts: ");
    loc.enc.printCounts();
  }

  // Encoder stuff
  // Using encoder data to keep the robot going straight
  // This is and incredibly basic first attempt
  if(loc.enc.getLeftCount() > loc.enc.getRightCount()) {
    loc.rMotorPower++;
    loc.rMotor->setSpeed(loc.rMotorPower);
  }
  if(loc.enc.getLeftCount() < loc.enc.getRightCount()) {
    loc.rMotorPower--;
    loc.rMotor->setSpeed(loc.rMotorPower);
  }
  
  // Sonar delay shouldn't really be in the main loop like this, it should be something that is handled 
  // by checking milis in a sonar object which is called to check and optionally update each time
  // through the loop
  delay(50); // 50ms delay between sonar pings, I think at least 30ms was suggested
  if(loops==0) {
    loc.stopMotors();
    for(;;) { delay(1000); }
  }

}
*/

