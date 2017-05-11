/**
 * @file Team BLRS Feedback Controller Library (FBC)
 *       > PID Controller Tools
 * @brief Contains algorithms for PID computation and initialization
 *
 * @author Jonathan Bayless, Elliot Berman, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FBC_PID_H_
#define _FBC_PID_H_

#include "fbc.h"

/**
 * Struct containing necessary data for the PID controller to function,
 * include the various constants necessary
 */
typedef struct fbc_pid {
  // The proprtional constant
  double kP;
  // The integral constant
  double kI;
  // The derivative constant
  double kD;
  // Minimum value the integral can take. This limits the effect of the integral
  int minI;
  // Maximum value the integral can take.
  int maxI;
  //**INTERNAL USE**
  long _integral;
  int  _prevError;
} fbc_pid_t;

void fbcPIDInitializeData(fbc_pid_t* fbc_pid, double kP, double kI, double kD,
  int minIntegral, int maxIntegral);

void fbcPIDInit(fbc_t* fbc, fbc_pid_t* config);


#endif /* end of include guard: _FFBC_PID_H_ */
