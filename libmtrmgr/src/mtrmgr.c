/**
 * @file Team BLRS Motor Manager Library (MtrMgr Library)
 * @brief Provides additional functionality for interfacing with the Vex motors.
 * Allows for output recalculation to account for the MC29s (trueSpeed), slewing,
 * and easier configuration for motor characteristics (reversal, etc.)
 *
 * @author Elliot Berman, Jonathan Bayless, Brian Hanford
 *
 */
#include "mtrmgr.h"

static Motor motor[10];
static Mutex mutex[10];
static TaskHandle motorManagerTaskHandle;


/**
 * @brief The default recalculate function for RecalculateCommanded (takes input and returns it)
 *        This method is only accessible to this file for organizational purposes and may be opened to other files.
 * @param in
 *        Input value
 * @returns Returns in
 */
static int _defaultRecalculate(int in) {
	return in;
}


/**
 * @brief The motor manager task processes all the motors and determines if a change
 * 				needs to be made to the motor speed and executes the change if necessary
 *        This task is initialized by the InitializeMotorManager method.
 *				Do not manually create this task.
 */

static void _motorManagerTask(void *none) {
	unsigned long int now;
	while (true) {
		now = millis();
		for (int i = 0; i < NUM_MOTORS; i++) {
			// the motorGet function gets a motor between channels 1-10. motor[index] goes from 0-9
			if (motorGet(i+1) != motor[i].cmd) {
				// Motor has not been set to target
				int current = motor[i]._prev;
				int commanded = motor[i].cmd;
				float slew = motor[i].slewrate;
				int out = 0;

				if (slew == 0) //setting a slew rate of zero prevents output
					continue;
				if (commanded != current) {
					//Slewing
					if (commanded > current) {
						out = current + (int)(slew * (millis() - motor[i]._lastUpdate)); //extrapolate largest allowable acceleration
						if (out > commanded) //requested change in output is lower than maximum possible
							out = commanded;
					}
					else if (commanded < current) {
						out = current - (int)(slew * (millis() - motor[i]._lastUpdate)); //extrapolate largest allowable acceleration
						if (out < commanded) //requested change in output is lower than maximum possible
							out = commanded;
					}

					motor[i]._prev = out;
					out = motor[i].recalculate(out);
				}

				// Grab mutex if possible, if it's not available (being changed by MotorSet()), skip the motor check.
				if (!mutexTake(mutex[i], 5))
					continue;
				motorSet(i+1, out);
				mutexGive(mutex[i]);
			}
			motor[i]._lastUpdate = millis();
		}
		taskDelayUntil(&now, SLEW_DELTA_T);
	}
}

void motorManagerInit() {
	for (int i = 0; i < NUM_MOTORS; i++) {
		mutex[i] = mutexCreate();
		motor[i].recalculate = &_defaultRecalculate;
	}
	motorManagerTaskHandle = taskCreate(_motorManagerTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_HIGHEST-1);
}

void blrsMotorInit(int port, bool inverted, float slewrate, int (*recalculate)(int)) {
	if (port < 1 || port > 10)
		return;
	port--;
	motor[port].port = port + 1;
	motor[port].inverted = inverted ? -1 : 1;
	motor[port].slewrate = slewrate;
	if(!recalculate) {
		motor[port].recalculate = &_defaultRecalculate;
	}
	else {
		motor[port].recalculate = recalculate;
	}
	motor[port]._prev = 0;
}

void motorManagerStop() {
	if (motorManagerTaskHandle != NULL) // passing NULL kills current thread, so don't allow that to happen
		taskDelete(motorManagerTaskHandle);
}

bool blrsMotorSet(int port, int commanded, bool immediate) {
	if (port > 10 || port < 1)
		return false;
	if (commanded > 127)
		commanded = 127;
	else if (commanded < -127)
		commanded = -127;
	port--;
	if (immediate) {
		// if not able to take control in time then exit and unsuccessful
		if (!mutexTake(mutex[port], MUTEX_TAKE_TIMEOUT)) {
			return false;
		}
		motorSet(port + 1, commanded * motor[port].inverted);
		mutexGive(mutex[port]);
	}
	motor[port].cmd = commanded * motor[port].inverted;
	return true;
}

int blrsMotorGet(int port) {
	if (port > 10 || port < 1)
		return 0;
	port--;
	return motor[port].cmd * motor[port].inverted;
}
