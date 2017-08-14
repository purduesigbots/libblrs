/**
 * @file Team BLRS Feedback Controller Library (FBCL)
 *       > Modified PID Controller Tools
 * @brief Contains algorithms for PID computation and initialization
 *
 * @author Jonathan Bayless, Elliot Berman, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "fbc_pidmod.h"

static int sgn(int x) {return (x > 0) ? 1 : ((x < 0) ? -1 : 0);}

static int _pidmodCompute(fbc_t* fbc, int error) {
  fbc_pidmod_t* data = (fbc_pidmod_t*)(fbc->_controllerData);

  if (sgn(error) != sgn(data->_prevError))
    data->_integral /= 2;
  else
    data->_integral += error;
  double derivative = ((double)(error - data->_prevError) / (CUR_TIME() - fbc->_prevExecution));
  data->_prevError = error;
  return (data->kP * error) + (data->kI * data->_integral) + (data->kD * derivative)
         + (data->kF * fbc->goal);
}

static void _pidmodReset(fbc_t* fbc) {
  fbc_pidmod_t * data = (fbc_pidmod_t*)(fbc->_controllerData);
  data->_integral = 0;
  data->_prevError = 0;
}

void fbcPIDModInitializeData(fbc_pidmod_t* fbc_pidmod, double kP, double kI, double kD,
                             double kF) {
  fbc_pidmod->kP = kP;
  fbc_pidmod->kI = kI;
  fbc_pidmod->kD = kD;
}

void fbcPIDModInit(fbc_t* fbc, fbc_pidmod_t* config) {
  fbc->compute = &_pidmodCompute;
  fbc->_controllerData = config;
  fbc->resetController = &_pidmodReset;
}
