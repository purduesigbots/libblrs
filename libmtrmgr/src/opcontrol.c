/** @file opcontrol.c
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
 * Runs the user operator control code. This function will be started in its own
 * task with the
 * default priority and stack size whenever the robot is enabled via the Field
 * Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is
 * disabled or
 * communications is lost, the operator control task will be stopped by the
 * kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the
 * VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX
 * Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX
 * Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is
 * available and
 * the scheduler is operational. However, proper use of delay() or
 * taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating
 * LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop,
 * even if empty.
 */
void operatorControl() {
  int power, turn;
  while (1) {
    power = joystickGetAnalog(1, 2); // vertical axis on left joystick
    turn = joystickGetAnalog(1, 1);  // horizontal axis on left joystick
    chassisSet(power + turn, power - turn);

    clawSet(joystickGetAnalog(1, 4));

    // add the following logic:
    if (joystickGetDigital(1, 6, JOY_UP)) {
      liftSet(127); // pressing up, so lift should go up
    } else if (joystickGetDigital(1, 6, JOY_DOWN)) {
      liftSet(-127); // pressing down, so lift should go down
    } else {
      liftSet(0); // no buttons are pressed, stop the lift
    }

    delay(20);
  }
}
