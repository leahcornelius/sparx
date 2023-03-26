/*
Copyright (C) 2023 by
    Leah Cornelius <leocornelius019@gmail.com> @ Cornelius Inovations

This file is part of the Sparx Robotics Project. It is free software; but attribution is required and you may not sell it or its derivatives.

This provides a simple interface for driving the two drive wheel/track motors using the LM298N motor driver.
 It is designed to run on an Arduino Nano (ATmega328P) and uses an in-house SerialManager library to communicate with the main controller.
The main controller is expected to send commands in the following format:
<command>;[param1],[param2]...[paramN],\n
where <command> is a string and <param> is an integer
Example:
sm;100,100,\n sets the left and right motor speeds to forwards with a PWM of 100 (out of 255 max)
*/

#include "SerialManager.h"


SM::SerialManager usb;

const int mR_pinA = 5;
const int mR_pinB = 4;
const int mR_ENA = 10;

const int mL_pinA = 8;
const int mL_pinB = 6;
const int mL_ENA = 9;

int left_speed = 0;
int right_speed = 0;

int deadzone = 25;
bool built_in_led_state = false;
bool speeds_changed = false;
bool brake_enabled = false;

void setup() {
  usb.start();
  Serial.println("sparx.movement.serial");
  pinMode(mR_pinA, OUTPUT);
  pinMode(mR_pinB, OUTPUT);
  pinMode(mL_pinA, OUTPUT);
  pinMode(mL_pinB, OUTPUT);
  pinMode(mR_ENA, OUTPUT);
  pinMode(mL_ENA, OUTPUT);
  analogWrite(mR_ENA, 0);
  analogWrite(mL_ENA, 0);
}

void set_motor_speeds(int left, int right) {
  if (right < 0 - deadzone) {
    // Reverse
    digitalWrite(mR_pinA, LOW);
    digitalWrite(mR_pinB, HIGH);
  } else if (right > deadzone) {
    digitalWrite(mR_pinA, HIGH);
    digitalWrite(mR_pinB, LOW);
  } else {
    right = 0;
  }

  if (left < 0 - deadzone) {
    // Reverse
    digitalWrite(mL_pinA, LOW);
    digitalWrite(mL_pinB, HIGH);
  } else if (left > deadzone) {
    digitalWrite(mL_pinA, HIGH);
    digitalWrite(mL_pinB, LOW);
  } else {
    left = 0;
  }

  analogWrite(mR_ENA, abs(right));
  analogWrite(mL_ENA, abs(left));
}

void enable_brakes(bool left, bool right) {
  if (right) {
    digitalWrite(mR_pinA, HIGH);
    digitalWrite(mR_pinB, HIGH);
    analogWrite(mR_ENA, 255);
  }

  if (left) {
    digitalWrite(mL_pinA, HIGH);
    digitalWrite(mL_pinB, HIGH);
    analogWrite(mL_ENA, 255);
  }
}

void printArgCountError(String cmd, int expected, int got) {
  Serial.print("ERROR: ");
  Serial.print(cmd);
  Serial.print(" takes ");
  Serial.print(expected, DEC);
  Serial.print(" arguments (got ");
  Serial.print(got, DEC);
  Serial.println(")");
}

void loop() {
  if (usb.onReceive()) {
    if (usb.command == "kill") {
      Serial.println("killing");
      set_motor_speeds(0, 0);
      while (true) {
        Serial.println("killed");
        delay(100);
      }
    } else if (usb.command == "sm") {
      if (usb.paramCount != 2) {
        printArgCountError("setmotors", 2, usb.paramCount);
      } else {
        brake_enabled = false;
        left_speed = usb.params[0].toInt();
        right_speed = usb.params[1].toInt();
        speeds_changed = true;
        Serial.print("ML: ");
        Serial.print(left_speed, DEC);
        Serial.print(", MR: ");
        Serial.println(right_speed, DEC);
      }
    } else if (usb.command == "tb") {
      brake_enabled = !brake_enabled;
      Serial.print("brake enabled: ");
      Serial.println(brake_enabled, DEC);
      left_speed = 0;
      right_speed = 0;
      speeds_changed = true;
    }
  }
  if (speeds_changed) {
    speeds_changed = false;
    if (brake_enabled) {
      enable_brakes(true, true);
    } else {
      set_motor_speeds(left_speed, right_speed);
    }
  }
}
