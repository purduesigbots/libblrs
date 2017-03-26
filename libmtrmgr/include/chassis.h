/** @file chassis.h
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

#ifndef _CHASSIS_H_
#define _CHASSIS_H_

// Sets the speeds of the left and right wheels of the chassis
void chassisSet(int left, int right);

void chassisInit();

#endif // _CHASSIS_H_
