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

#include "main.h"

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost,  the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
void autonomous() {
  chassisSet(127, 127); // drive forward
  delay(3500); // delay 3.5 seconds = 3500 milliseconds
  chassisSet(0, 0); // stop driving forward
  liftSet(127); // lift goes up
  delay(2000);
  liftSet(0);
  for(int i = 0; i < 3; i++) {
    chassisSet(-127, 127); // rotate left
    delay(1000);
    chassisSet(127, 127); // rotate right for twice as long \\--/
    delay(2000);
  }
  chassisSet(0, 0); // stop chassis for safety, although potentially not necessary
}
