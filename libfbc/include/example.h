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
#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_

#include <API.h>

#define EXAMPLE_DB 15 // the motors don't do much below this speed
#define EXAMPLE_TOLERANCE 20 // Depends heavily on the system
#define EXAMPLE_CONFIDENCE 5 // Means that the controller needs to be in the tolerance band for ~200ms, usually enough

#define EXAMPLE_KP 1.0f
#define EXAMPLE_KI 0.001f
#define EXAMPLE_KD 100.0f
#define EXAMPLE_INTEG_CAP 100000

void exampleInit();
void exampleSetPos(int position);
void exampleMoveToPos();

#endif
