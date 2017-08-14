/**
 * @file Team BLRS Feedback Controller Library (FBC)
 *       > Modified PID Controller Tools
 * @brief Contains algorithms for PID computation and initialization
 *
 * @author Jonathan Bayless, Elliot Berman, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FBC_PIDMOD_H_
#define _FBC_PIDMOD_H_

#include "fbc.h"

/**
 * Struct containing necessary data for the PID controller to function,
 * include the various constants necessary
 */
typedef struct fbc_pidmod {
  // The proprtional constant
  double kP;
  // The integral constant
  double kI;
  // The derivative constant
  double kD;
  // The feedforward constant for velocity control applications
  double kF;
  //**INTERNAL USE**
  long _integral;
  int  _prevError;
} fbc_pidmod_t;

void fbcPIDModInitializeData(fbc_pidmod_t* fbc_pidmod, double kP, double kI, double kD, double kF);

void fbcPIDModInit(fbc_t* fbc, fbc_pidmod_t* config);

#endif /* end of include guard: _FBC_PIDMOD_H_ */
