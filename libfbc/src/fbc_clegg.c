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

#include "fbc_clegg.h"

static int sgn(int x) {return (x > 0) ? 1 : ((x < 0) ? -1 : 0);}

static int _cleggCompute(fbc_t* fbc, int error) {
  fbc_clegg_t* data = (fbc_clegg_t*)(fbc->_controllerData);

  data->_integral += error;

  int out;
  if (fbc->goal != data->_prevGoal) {
    out = sgn(fbc->goal) * data->initialOutput;
    data->_integral = 0;
  }
  else if (sgn(error) != sgn(data->_prevError)) {
    out = 0;
    data->_integral = 0;
  }
  else
    out = data->gain * data->_integral;

  return out;
}

static void _cleggReset(fbc_t* fbc) {
  fbc_clegg_t* data = (fbc_clegg_t*)(fbc->_controllerData);
  data->_integral = 0;
  data->_prevError = 0;
  data->_prevGoal = 0;
}

void fbcCleggInitializeData(fbc_clegg_t* fbc_clegg, double gain, int initialOutput) {
  fbc_clegg->gain = gain;
  fbc_clegg->initialOutput = initialOutput;
  fbc_clegg->_integral = 0;
  fbc_clegg->_prevError = 0;
  fbc_clegg->_prevGoal = 0;
}

void fbcCleggInit(fbc_t* fbc, fbc_clegg_t* fbc_clegg) {
  fbc->compute = &_cleggCompute;
  fbc->_controllerData = fbc_clegg;
  fbc->resetController = &_cleggReset;
}
