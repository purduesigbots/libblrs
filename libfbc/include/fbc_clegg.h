/**
 * @file Team BLRS Feedback Controller Library (FBC)
 *       > Clegg Integrator Tools
 * @brief Contains algorithms for Clegg Integrator computation and initialization
 *
 * @author Jonathan Bayless, Elliot Berman, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _FBC_CLEGG_H_
#define _FBC_CLEGG_H_

#include "fbc.h"

/**
 * Struct containing the various data necessary for the Clegg Integrator to function
 */
typedef struct fbc_clegg {
  // The gain constant for the integrating action
  double gain;
  // When the fbc's goal changes, the output is set to this value and then the controller
  // intregrates from there
  unsigned int initialOutput;
  //**INTERNAL USE**
  long _integral;
  int _prevGoal, _prevError;
} fbc_clegg_t;

void fbcCleggInitializeData(fbc_clegg_t* fbc_clegg, double gain, int initialOutput);

void fbcCleggInit(fbc_t* fbc, fbc_clegg_t* fbc_clegg);

#endif /* end of include guard: _FBC_CLEGG_H_ */
