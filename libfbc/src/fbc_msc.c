/**
 * @file Team BLRS Feedback Controller Library (FBCL)
 *       > Master Slave Controller Tools
 * @brief Contains algorithms for PID computation and initialization
 *
 * @author Jonathan Bayless, Elliot Berman, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "fbc_msc.h"

static int mscSense(msc_t* msc) { return msc->master->sense() - msc->slave->sense(); }

static void _mscTask(void* param) {
	msc_t* msc = (msc_t*)param;
	unsigned long now = millis();
	while(true) {
		mscRunContinuous(msc);
		taskDelayUntil(&now, FBC_LOOP_INTERVAL);
	}
}

void mscInitializeData(msc_t* msc, fbc_t* master, fbc_t* slave, double eq_kP, double eq_kI, double eq_kD,
                       int eq_minIntegral, int eq_maxIntegral) {
	msc->master = master;
	msc->slave = slave;
	msc->man_out = 0;
	msc->manual = false;

	fbc_t* eq = (fbc_t*)malloc(sizeof(fbc_t));
	fbc_pid_t* pid = (fbc_pid_t*)malloc(sizeof(fbc_pid_t));
	pid->kP = eq_kP;
	pid->kI = eq_kI;
	pid->kD = eq_kD;
	pid->minI = eq_minIntegral;
	pid->maxI = eq_maxIntegral;
	fbcPIDInit(eq, pid);
	msc->_eq = eq;
}

void mscInit(msc_t* msc, fbc_t* master, fbc_t* slave, fbc_t* eq) {
	msc->master = master;
	msc->slave = slave;
	msc->_eq = eq;
}

int mscGenerateOutput(msc_t* msc, bool master) {
	// The linter yells about the nested function but gcc is cool with it
	int sense() { return mscSense(msc); }
	msc->_eq->sense = sense;

	if (msc->manual && master) {
		int master = msc->man_out;
		master -= fbcGenerateOutput(msc->_eq);
		return master;
	}
	else if (msc->manual && !master) {
		int slave = msc->man_out;
		slave += fbcGenerateOutput(msc->_eq);
		return slave;
	}
	else if (!msc->manual && master) {
		int master = fbcGenerateOutput(msc->master);
		master -= fbcGenerateOutput(msc->_eq);
		return master;
	}
	else {
		int slave = fbcGenerateOutput(msc->slave);
		slave += fbcGenerateOutput(msc->_eq);
		return slave;
	}
}

int mscIsConfident(msc_t* msc) {
	if (fbcIsConfident(msc->master) == -1 || fbcIsConfident(msc->slave) == -1)
		return -1; //return stalled if either is stalled
	return fbcIsConfident(msc->master) * fbcIsConfident(msc->slave);
}

void mscReset(msc_t* msc) {
	fbcReset(msc->master);
	fbcReset(msc->slave);
	fbcReset(msc->_eq);
}

void mscSetGoal(msc_t* msc, int new_goal, bool manual) {
	if (manual) {
		msc->man_out = new_goal;
		return;
	}
	fbcSetGoal(msc->master, new_goal);
	fbcSetGoal(msc->slave, new_goal);
}

int mscRunContinuous(msc_t* msc) {
	msc->master->move(mscGenerateOutput(msc, true));
	msc->slave->move(mscGenerateOutput(msc, false));
	return mscIsConfident(msc);
}

bool mscRunCompletion(msc_t* msc, unsigned long timeout) {
	unsigned long now = millis();
	unsigned long start = millis();
#define HAS_TIMED_OUT   (timeout == 0 || (start + timeout) >= now)
	while(!mscRunContinuous(msc) && HAS_TIMED_OUT)
		taskDelayUntil(&now, FBC_LOOP_INTERVAL);
	return HAS_TIMED_OUT;
#undef HAS_TIMED_OUT
}

TaskHandle mscRunParallel(msc_t* msc) {
	return taskCreate(_mscTask, TASK_DEFAULT_STACK_SIZE, msc, TASK_PRIORITY_DEFAULT);
}
