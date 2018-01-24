/** @file example.h
 *
 * ========= DEMO CODE =========
 * This file contains example code using BLRS's libfbc
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */
#include "example.h"
#include "fbc.h"
#include "fbc_pid.h"

fbc_t exampleController;
fbc_pid_t examplePID;

static void _exampleMove(int power) {
  motorSet(1, power);
}

static int _exampleSense() {
  return analogRead(1);
}

static void _exampleReset() {
  // If using a sensor like an encoder that needs reset, do that here.
}

void exampleInit() {
  fbcInit(&exampleController, _exampleMove, _exampleSense, _exampleReset,
          fbcStallDetect, -EXAMPLE_DB, EXAMPLE_DB, EXAMPLE_TOLERANCE, EXAMPLE_CONFIDENCE);
  fbcPIDInitializeData(&examplePID, EXAMPLE_KP, EXAMPLE_KI, EXAMPLE_KD, -EXAMPLE_INTEG_CAP, EXAMPLE_INTEG_CAP);
  fbcPIDInit(&exampleController, &examplePID);
}

void exampleSetPos(int position) {
  fbcSetGoal(&exampleController, position);
}

void exampleMoveToPos() {
  fbcRunCompletion(&exampleController, 5000); // 5 second timeout on the movement
}
