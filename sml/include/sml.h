/**
 * @file PROS Smart Motor Library (SML)
 * @brief Provides additional functionality for interfacing with the Vex motors.
 * Allows for linear scaling to account for the MC29s (trueSpeed), slewing,
 * and easier configuration for motor characteristics (reversal, etc.)
 *
 * @author Elliot Berman, Jonathan Bayless, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _SML_H_
#define _SML_H_

#include <API.h>

#define SLEW_DELTA_T 20 //the update rate for the motors in ms
#define NUM_MOTORS 10
#define MOTOR_DEADBAND 7 //a pwm value of less than 8 will result in zero output to the motors
#define DEFAULT_SLEW 0.5f //the recommended slew rate for 393 motors

#define MUTEX_TAKE_TIMEOUT 100

/**
 * Lookup table for True Speed calculation
 * This will need tuned for each subsystem to achieve a perfectly linear response,
 * but these values should be close enough for most applications.
 */
extern const unsigned int trueSpeedLUT[128];

/*
* This defines a "Motor" and all of its corresponding information.
* Most of this information is only used internally within SML.
*/
typedef struct {
  unsigned char port;         // port number of given motor
  int pwm;                    // current pwm value
  int cmd;                    // commanded pwm value
  unsigned long _lastUpdate;  // time (msec) of last commanded value
  float slewrate;             // caps the motor's acceleration
  char inverted;              // flips the motor output to avoid needing to electrically flipping motors
  bool immediate;             // bypasses the slewrate and immediately sets to desired motor power
  int(*recalculate)(int);     // used to scale the motor output for trueSpeed or other scalings
} Motor;

/**
 * @brief Initializes the Motor Manager Task by creating the Motor Mutexes and starting the task.
 *        NOTE: MUST BE CALLED IN "INIT.C" TO USE SML
 */
void motorManagerInit();

/**
 * @brief Kills the motor manager task, if it exists
 */
void motorManagerStop();

/**
 * @brief Configures a motor port with inversion, slew, and scaling
 *
 * @param channel
 *        The port of the motor [1,10]
 *
 * @param inverted
 *        If the motor port is inverted, then set to true (127 will become -127 and vice versa)
 *
 * @param slew
 *        The acceleration of the motor in dPWM/millisecond. DEFAULT_SLEW_RATE is available, which sets dPWM/millisecond to 0.75
 *
 * @param recalculate
 *        function pointer to the scaling function for the motor. The default function (NULL) doesn't affect the output at all.
 *        passing "trueSpeed" here activates true speed recalculation
 */
void smartMotorInit(int port, bool inverted, float slew, int (*recalculate)(int));

/**
 * @brief Change the motor speed
 *
 * @param channel
 *        The port of the motor (1-10)
 *
 * @param commanded
 *        The PWM value of the motor to set it to. Will be checked and forced back into the bounds [-127,127]
 *
 * @param immediate
 *        Will change the speed of the motor immediately, bypassing the motor manager ramping if set to true.
 *
 * @returns Returns true if MotorSet was successful.
 */
bool smartMotorSet(int port, int speed, bool immediate);

/**
 * @brief Returns the normalized commanded speed of the motor
 *
 * @param channel
 *          The port of the motor [1,10]
 *
 * @returns Returns the cmd speed of the motor
 */
int smartMotorGet(int port);

/**
 * @brief linearizes the motor output to account for the MC29s
 *
 * @param cmd
 *        the set pwm value, pre-scaling
 *
 * @returns returns the linearized pwm output value
 *
*/
int trueSpeed(int cmd);

#endif
