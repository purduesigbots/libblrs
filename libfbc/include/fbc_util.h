/**
 * @file Team BLRS Feedback Controller Library (FBC)
 *       > General Tuning Tools
 * @brief Contains algorithms for PID Autotuning and Deadband Finding
 *
 * @author Jonathan Bayless
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef _FBC_UTIL_H_
#define _FBC_UTIL_H_

#include "fbc.h"
#include "fbc_pid.h"
#include <API.h>

// The weighting constant for the particles inertia
#define INERTIA 0.5f

// The weighting constant for the particle's self confidence
#define CONF_SELF 1.1f

// The weighting constant for the particle's confidence in the swarm of particles
#define CONF_SWARM 1.2f

// ITAE constant used with fbcPIDAutotuneSimple()
#define DEFAULT_K_ITAE 2

// Settle time constant used with fbcPIDAutotuneSimple()
#define DEFAULT_K_SETTLE 1

// Number of particles used with fbcPIDAutotuneSimple()
#define DEFAULT_NUM_PARTICLES 16

// Number of iterations used with fbcPIDAutotuneSimple()
#define DEFAULT_NUM_ITERATIONS 5

/**
 * @brief Finds the optimal tuning for the fbc using a PSO Artificial Intelligence algorithm.
 *        The robot will move back and forth executing a series of test movements to determine
 *        the best set of constants through a variety of measurements. These constants will either
 *        be printed to the LCD and terminal or simply the terminal.
 *
 * @param fbc
 *        The controller to be tuned.
 * @param num_iterations
 *        The number of times each particle will be tested and updated
 * @param num_particles
 *          The number of randomized PID tunings that will be tested each iteration.
 *          A number of particles between 10 and 30 (the maximum number allowed) is ideal.
 * @param timeout
 *          How long the movement can last before the routine will move to the next particle
 * @param goal1
 *          The target for the feedback controller to move to on a first iteration.
 * @param gaol2
 *          The target for the feedback controller to move to on a second iteration.
 * @param lcd
 *          An LCD port for printing the constants. Passing NULL will only print to the terminal.
 * @param kP_min
 *          The minimum kP value for the particles.
 * @param kP_max
 *          The maximum kP value for the particles.
 * @param kI_min
 *          The minimum kI value for the particles.
 * @param kI_max
 *          The maximum kI value for the particles.
 * @param kD_min
 *          The minimum kD value for the particles.
 * @param kD_max
 *          The maximum kD value for the particles.
 * @param k_settle
 *          A constant for the fitness function to affect the weighting of the settling time in
 *          determining the best set of constants
 * @param k_itae
 *	      The weighting contant for the summed error component for determining the best constants.
 */
void fbcPIDAutotuneFull(fbc_t* fbc, int num_iterations, int num_particles, int timeout, int goal1, int goal2, FILE* lcd,
                        double kP_min, double kP_max, double kI_min, double kI_max, double kD_min, double kD_max,
                        double k_settle, double k_itae);

/**
 * @brief A simplified version of fbcPIDAutotuneFull() that sets defaults for a number of the parameters
 *
 * @param fbc
 *        The controller to be tuned.
 * @param timeout
 *          How long the movement can last before the routine will move to the next particle
 * @param goal
 *          The target for the feedback controller to move to.
 * @param lcd
 *          An LCD port for printing the constants. Passing NULL will only print to the terminal.
 * @param kP_min
 *          The minimum kP value for the particles.
 * @param kP_max
 *          The maximum kP value for the particles.
 * @param kI_min
 *          The minimum kI value for the particles.
 * @param kI_max
 *          The maximum kI value for the particles.
 * @param kD_min
 *          The minimum kD value for the particles.
 * @param kD_max
 *          The maximum kD value for the particles.
 */
void fbcPIDAutotuneSimple(fbc_t* fbc, int timeout, int goal, FILE* lcd, double kP_min, double kP_max, double kI_min,
                          double kI_max, double kD_min, double kD_max);

/**
 * @brief Gradually increments the fbc's output until the desired change in sense in achieved,
 *        indicating the system's deadband is reached.
 *
 * @param fbc
 *          The feedback controller to be evaluated.
 * @param delta_sense
 *          The change in the sense value over the half second movement that indicates success
 * @param lcd
 *           The lcd port to print the values to, passing NUll will just print to terminal
 */
void fbcFindDeadband(fbc_t* fbc, int delta_sense, FILE* lcd);

#endif
