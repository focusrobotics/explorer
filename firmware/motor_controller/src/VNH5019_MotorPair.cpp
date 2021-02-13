/****************************************************************************
 * Copyright (c) 2020 by Focus Robotics. All rights reserved.
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
 * Creation_Date:  Wed Dec 30 2020
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
#ifdef VNH5019_MOTOR

#include <VNH5019_MotorPair.h>
#include <Arduino.h>

VNH5019_MotorPair::VNH5019_MotorPair() {}

VNH5019_MotorPair::~VNH5019_MotorPair() {}

void VNH5019_MotorPair::setup() {
  //md.init();
  _INA1 = PB12;
  _INB1 = PB13;
  _PWM1 = PA9;
  _INA2 = PB14;
  _INB2 = PB15;
  _PWM2 = PA8;
  pinMode(_INA1, OUTPUT);
  pinMode(_INB1, OUTPUT);
  pinMode(_PWM1, OUTPUT);
  pinMode(_INA2, OUTPUT);
  pinMode(_INB2, OUTPUT);
  pinMode(_PWM2, OUTPUT);
  analogWriteFrequency(5000);
  analogWrite(_PWM1, 0);
  analogWrite(_PWM2, 0);
}

void VNH5019_MotorPair::set_speed(motor_select_t mot, int16_t speed) {
  //  md.setM1Speed(leftSpeed); // Accepts -400 to +400
  //  md.setM2Speed(rightSpeed);
  // input speed is +/-1023. To scale to +/-400 I must divide by 2.5
  int16_t pow = abs(speed) / 4;
  if(mot==RIGHT_MOTOR) {
    //md.setM1Speed(speed/2.5);
    if(speed>0) { // clockwise for positive speed
      digitalWrite(_INA1, HIGH);
      digitalWrite(_INB1, LOW);
      analogWrite(_PWM1, pow);

    } else if(speed<0) { // counterclockwise for negative speed
      digitalWrite(_INA1, LOW);
      digitalWrite(_INB1, HIGH);
      analogWrite(_PWM1, pow);

    } else { // speed == 0
      digitalWrite(_INA1, LOW);
      digitalWrite(_INB1, LOW);
      analogWrite(_PWM1, pow);

    }
    Serial.print("Set m1 to ");
  } else {
    //md.setM2Speed(speed/2.5);
    if(speed>0) { // clockwise for positive speed
      digitalWrite(_INA2, HIGH);
      digitalWrite(_INB2, LOW);
      analogWrite(_PWM2, pow);

    } else if(speed<0) { // counterclockwise for negative speed
      digitalWrite(_INA2, LOW);
      digitalWrite(_INB2, HIGH);
      analogWrite(_PWM2, pow);

    } else { // speed == 0
      digitalWrite(_INA2, LOW);
      digitalWrite(_INB2, LOW);
      analogWrite(_PWM2, pow);

    }
    Serial.print("Set m2 to ");
  }
  Serial.println(pow);
}

#endif
