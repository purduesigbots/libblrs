/** @file chassis.c
 *
 * ========= DEMO CODE =========
 * This file contains example code from the clawbot tutorial using BLRS's mtrmgr
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"    // includes API.h and other headers
#include "chassis.h" // redundant, but ensures that the corresponding header file (chassis.h) is included
#include "ports.h"

void chassisSet(int left, int right) {
  blrsMotorSet(CHASSIS_LEFT_MOTOR, left, false);
  blrsMotorSet(CHASSIS_RGHT_MOTOR, right, false);
}

int chassisTrueSpeed(int cmd) {
  static const unsigned int TrueSpeed[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 25, 25, 25, 25, 26, 26, 26,
    26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29, 29, 29,
    29, 29, 29, 30, 30, 30, 30, 30, 31, 31, 31, 31, 32, 32, 32, 33, 33, 33, 33,
    34, 34, 34, 34, 35, 35, 35, 35, 36, 36, 36, 37, 37, 37, 37, 38, 38, 38, 39,
    39, 39, 40, 40, 41, 42, 42, 42, 42, 42, 43, 43, 44, 44, 45, 45, 46, 47, 47,
    47, 48, 48, 49, 49, 50, 50, 52, 52, 52, 53, 54, 55, 55, 56, 56, 60, 61, 64,
    65, 65, 66, 70, 72, 72, 80, 85, 127, 127
    }; //True Speed array to linearize motor output

  return (cmd > 0 ? TrueSpeed[cmd] : (-1 *  TrueSpeed[cmd]));
}

void chassisInit() {
  // CHASSIS_LEFt_MOTOR is inverted, can accelerate as quickly as 3.0 PWM/millisecond and uses truespeed
  blrsMotorInit(CHASSIS_LEFT_MOTOR, true, 3.0, &chassisTrueSpeed);
  // CHASSIS_RIGHT_MOTOR is uninverted, can accelerate as quickly as 3.0 PWM/millisecond and uses truespeed
  blrsMotorInit(CHASSIS_RGHT_MOTOR, false, 3.0, &chassisTrueSpeed);
}
