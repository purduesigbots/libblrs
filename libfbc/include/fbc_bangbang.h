/**
 * @file Team BLRS Feedback Controller Library (FBC Library)
 *       > BangBang Controller Tools
 * @brief Contains algorithms for BangBang-esque computation and initialization
 *
 * This is not a traditional BangBang control implementation; traditional BangBang control tends to be too violent for
 * use with VEX subsystems. This implementation uses a Proportional controller for braking instead of switching to a set
 * opposing speed when the goal gets crossed, which results in much smoother movements that appropriately account for
 * inertia.
 *
 * @author Jonathan Bayless
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FBC_BANGBANG_H_
#define _FBC_BANGBANG_H_

#include "fbc.h"

/**
 * Struct containing necessary data for the BangBang controller to function,
 * include the various constants necessary
 */
typedef struct fbc_bangbang {
	// The braking constant for when the system has passed its goal
	double kBrake;
	// Normal output speed when not at goal
	int fullSpeed;

	//**INTERNAL USE**
	bool _passed;
} fbc_bangbang_t;

/**
 * @brief Initializes the constants for a BangBang controller.
 *
 * @param bangbang
 *        The BangBang controller to be initialized
 * @param kBrake
 *        The braking constant, operates like a Proportional controller when the system is past its goal
 * @param fullSpeed
 *        The pre-goal operating speed for the system. Setting this to 127 is common, but a lower speed will result in
 * smoother movements.
 */
void fbcBangBangInitializeData(fbc_bangbang_t* bangbang, double kBrake, int fullSpeed);

/**
 * @brief Configures the given FBC to be a BangBang controller
 *
 * @param fbc
 *        The FBC to be configured
 * @param config
 *        The BangBang controller used to configure the FBC
 */
void fbcBangBangInit(fbc_t* fbc, fbc_bangbang_t* config);

#endif /* end of include guard: _FBC_BANGBANG_H_ */
