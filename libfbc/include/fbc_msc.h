/**
 * @file Team BLRS Feedback Controller Library (FBC Library)
 *       > Master Slave Controller Tools
 * @brief Contains algorithms for PID computation and initialization
 *
 * @author Jonathan Bayless, Elliot Berman
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _FBC_MSC_H_
#define _FBC_MSC_H_

#include "fbc.h"
#include "fbc_pid.h"

typedef struct msc msc_t;
typedef struct msc {
	fbc_t* master;
	fbc_t* slave;

	/**
	 * Internal Data
	 */
	fbc_t* _eq;
	int man_out;
	bool manual;
} msc_t;

void mscInitializeData(msc_t* msc, fbc_t* master, fbc_t* slave, double eq_kP, double eq_kI, double eq_kD,
                       int eq_minIntegral, int eq_maxIntegral);
void mscInit(msc_t* msc, fbc_t* master, fbc_t* slave, fbc_t* eq);


int mscGenerateOutput(msc_t* msc, bool master);
int mscRunContinuous(msc_t* msc);
int mscIsConfident(msc_t* msc);
void mscReset(msc_t* msc);
void mscSetGoal(msc_t* msc, int new_goal, bool manual);
bool mscRunCompletion(msc_t* msc, unsigned long timeout);
TaskHandle mscRunParallel(msc_t* msc);

#endif
