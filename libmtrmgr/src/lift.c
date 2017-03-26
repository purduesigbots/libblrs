/** @file lift.c
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

#include "main.h"
#include "ports.h"

void liftSet(int speed) {
  blrsMotorSet(LIFT_MOTOR, speed, false);
}

void liftInit() {
  blrsMotorInit(LIFT_MOTOR, false, DEFAULT_SLEW_RATE, NULL);
}
