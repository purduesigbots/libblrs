/**
 * @file Team BLRS Feedback Controller Library (FBCL)
 *       > PID Controller Tools
 * @brief Contains algorithms for PID computation and initialization
 *
 * @author Jonathan Bayless, Elliot Berman, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "fbc_pid.h"

static int _pidCompute(fbc_t* fbc, int error) {
	fbc_pid_t* data = (fbc_pid_t*)(fbc->_controllerData);

	data->_integral += error;
  	if(data->_integral < data->minI)
    		data->_integral = data->minI;
  	else if(data->_integral > data->maxI)
    		data->_integral = data->maxI;
  	double derivative = ((double)(error - data->_prevError) / (CUR_TIME() - fbc->_prevExecution));
  	data->_prevError = error;
  	return (data->kP * error) + (data->kI * data->_integral) + (data->kD * derivative);
}

static void _pidReset(fbc_t* fbc) {
	fbc_pid_t * data = (fbc_pid_t*)(fbc->_controllerData);
  	data->_integral = 0;
  	data->_prevError = 0;
}

void fbcPIDInitializeData(fbc_pid_t* fbc_pid, double kP, double kI, double kD,
                          int minIntegral, int maxIntegral) {
	fbc_pid->kP = kP;
	fbc_pid->kI = kI;
	fbc_pid->kD = kD;
	fbc_pid->maxI = maxIntegral;
	fbc_pid->minI = minIntegral;
}

void fbcPIDInit(fbc_t* fbc, fbc_pid_t* config) {
	fbc->compute = &_pidCompute;
	fbc->_controllerData = config;
	fbc->resetController = &_pidReset;
}
