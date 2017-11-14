/**
 * @file Team BLRS Feedback Controller Library (FBC Library)
 * @brief Provides components to implement error-based feedback controllers
  *       with pre-made templates for PID, TBH, and MS-PID
 *
 * @author Jonathan Bayless, Elliot Berman, Brian Hanford
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef _FBC_H_
#define _FBC_H_

#include <API.h>
#define FBC_LOOP_INTERVAL 20

/**
 * The error code returned by fbcIsConfident() and fbcRunContinuous when the robot stalls
 */
#define FBC_STALL -1

typedef struct fbc fbc_t; // predefine fbc_t for use inside fbc_t
 /**
  * The classical error-based closed-loop feedback controller is implemented in by fbc functions.
  * For simplicity and convenience, some naming conventions have been adopted to lower the learning curve.
  *
  * fbc provides mechanisms to interact with closed loop controllers and implements the basic feedback
  * controller plant (move), sensor (sense), and controller (compute) transfer functions. Additionally,
  * fbc determines if the system is on target through the use of a acceptTolerance and acceptConfidence.
  * acceptTolerance is the maximum error from the current sensor state to the goal state to be considered
  * on target. acceptConfidence determines the number of iterations the controller must be contiguously on
  * target to be considered stable and on target.
  */
 typedef struct fbc {
   // A function pointer to modify the system in accordance with the output of the controller
   void (*move)(int);
   // A function pointer to get the current state of the system (i.e. sensor input)
   int (*sense)(void);
   // A function pointer to compute the transfer function of the plant system
   int (*compute)(fbc_t*, int);
   // A function pointer to reset the state of the system (may do nothing)
   void (*resetSense)(void);
   // A function pointer to reset the state of the controller
   void (*resetController)(fbc_t*);
   // A function pointer to detect stall conditions
   // fbcStallDetect is available as a sample stall detectionn function but you can write your own
   bool (*stallDetect)(fbc_t*);

   int goal, output;
   int pos_deadband, neg_deadband;
   unsigned int acceptableConfidence, acceptableTolerance;
   bool confident;

   /*
   * FOR INTERNAL USE
   */
   void* _controllerData; // Controller data

   unsigned int _confidence;
   unsigned long _prevExecution; // most recent time of execution
   int _prevSense;
 } fbc_t;

/**
 * @brief a simple stall detection algorithm
 *
 * This stall detection algorithm looks for unexpectedly low movement for a given output, indicating that the robot
 * is experiencing significant enough resistance that it may cause a stall. This function pointer can be given
 * to the fbcInit function, or you can write your own stall detection function if you prefer.
 *
 * @param fbc
 *        A pointer to a feedback controller
 *
 * @returns true if the controller is stalled, false otherwise
 */
extern bool (*fbcStallDetect)(fbc_t* fbc);

/**
 * @brief Initializes a barebones feedback controller without the actual controller. Use an auxiliary function
 *        or manually set up the controller to actually execute it.
 *
 * @param fbc
 *        A pointer to a feedback controller
 * @param move
 *        A pointer to a function to set the motors/output of the system
 * @param sense
 *        A pointer to a function which retrieves sensor input
 * @param resetSense
 *        An optional pointer to a function which resets the sensor to a default value.
 *        If NULL, then nothing is done
 * @param stallDetect
 *        A pointer to a function that returns true if the controller has stalled
 *        fbcStallDetect is available as a standard option, or you can write your own
 * @param neg_deadband
 *        The lowest possible non-zero output value for the robot to move in the negative direction
 * @param pos_deadband
 *        The lowest possible non-zero output value for the robot to move in the positive direction
 * @param acceptableTolerance
 *        Maximum delta between the current sensor value and the goal sensor value to be considered on target
 *        for a given time slice
 * @param acceptableConfidence
 *        Minimum number of contiguous time slices that need to be on target to consider the system stably
 *        on target.
 */
void fbcInit(fbc_t* fbc, void (*move)(int), int (*sense)(void), void (*resetSense)(void),
             bool (*stallDetect)(fbc_t*), int neg_deadband, int pos_deadband, int acceptableTolerance,
             unsigned int acceptableConfidence);

/**
 * @brief Resets the feedback controller's goal, sensor state, and accumulating parameters, if applicable.
 */
void fbcReset(fbc_t* fbc);

/**
 * @brief Updates the feedback controller's goal. Additionally, will reset the controller as definied in
 *        fbc_reset. If new_goal is not different, then nothing is done.
 * @returns true if the operation was successful
 */
bool fbcSetGoal(fbc_t* fbc, int new_goal);

/**
 * @brief Reports the status of the controller - running, confident, or stalled
 *
 * @returns 1 if confident, FBC_STALL (-1) if stalled, and 0 otherwise
 */
int fbcIsConfident(fbc_t* fbc);

/**
 * @brief Runs one iteration of the feedback controller. This function is typically used when multiple
 *        controllers want to be run simulatenously on a single task.
 *
 * @returns 1 if confident, FBC_STALL (-1) if stalled, and 0 otherwise
 */
int fbcRunContinuous(fbc_t* fbc);

/**
 * @brief Runs the feedback controller in this task until it is stably on target.
 *        This function is blocking and will not exit if the controller cannot reach its goal.
 *
 * @param timeout
 *        Number of milliseconds that the controller will be allowed to run. Setting to 0 will disable timeout
 *
 * @note The implementation of this function is to call fbc_run_continuous every 25 milliseconds to completion
 * @note Timeout is only checked once every iteration. This means that if the controller times out, the amount
 *       of time passed may not be timeout milliseconds.
 *
 * @returns true if the movement timed out, false otherwise
 */
bool fbcRunCompletion(fbc_t* fbc, unsigned long int timeout);

/**
 * @brief Spawns a new task for parallel execution of the feedback controller.
 *
 * @note Use the PROS task management tools to pause/resume execution of this task
 */
TaskHandle fbcRunParallel(fbc_t* fbc);

/**
 * @brief Generates the output for the feedback controller but does not actually set the output (as opposed to fbcRunContinuous)
 */
int fbcGenerateOutput(fbc_t * fbc);


// Helper macros for implementations of fbc
// time is all done in microseconds
#define CUR_TIME  micros
#define TIME_TO_SEC(t) ((t) / 1000000)
#endif /* end of include guard: _FBC_H_ */
