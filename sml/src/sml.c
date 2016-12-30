/**
 * @file PROS Smart Motor Library (SML)
 * @brief Provides additional functionality for interfacing with the Vex motors.
 * Allows for linear scaling to account for the MC29s (trueSpeed), slewing,
 * and easier configuration for motor characteristics (reversal, etc.)
 *
 * @author Elliot Berman, Jonathan Bayless, Brian Hanford
 *
 */
#include "sml.h"

static Motor motor[10];
static Mutex mutex[10];
static TaskHandle motorManagerTaskHandle;
const unsigned int trueSpeedLUT[128] = {
  0, 0, 0, 0, 0, 0, 0, 10, 12, 15, 18, 20, 21,
  21, 22, 22, 22, 23, 24, 24, 25, 25, 25, 25, 26, 27, 27, 28, 28, 28, 28,
  29, 30, 30, 30, 31, 31, 32, 32, 32, 33, 33, 34, 34, 35, 35, 35, 36, 36,
  37, 37, 37, 37, 38, 38, 39, 39, 39, 40, 40, 41, 41, 42, 42, 43, 44, 44,
  45, 45, 46, 46, 47, 47, 48, 48, 49, 50, 50, 51, 52, 52, 53, 54, 55, 56,
  57, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 67, 68, 70, 71, 72, 72,
  73, 74, 76, 77, 78, 79, 79, 80, 81, 83, 84, 84, 86, 86, 87, 87, 88, 88,
  89, 89, 90, 90, 127, 127, 127
};

/**
 * @brief The default recalculate function for RecalculateCommanded (takes input and returns it)
 *        This method is only accessible to this file for organizational purposes and may be opened to other files.
 * @param in
 *        Input value
 * @returns Returns in
 */
static int defaultRecalculate(int in)
{
    return in;
}

/**
 * @brief linearizes the motor output to account for the MC29s
 *
 * @param cmd
 *        the set pwm value, pre-scaling
 *
 * @returns returns the linearized pwm output value
 *
*/
int trueSpeed(int cmd)
{
  return(cmd > 0 ? trueSpeedLUT[cmd] : (-1 *  trueSpeedLUT[cmd]));
}
/**
 * @brief The motor manager task processes all the motors and determines if a change
 * 				needs to be made to the motor speed and executes the change if necessary
 *        This task is initialized by the InitializeMotorManager method.
 *				Do not manually create this task.
 */
static void _motorManagerTask(void *none)
{
    unsigned long now;
    while (true)
    {
        now = millis();
        for (int i = 0; i < NUM_MOTORS; i++)
        {
					  // the motorGet function gets a motor between channels 1-10. motor[index] goes from 0-9
            if (motorGet(i+1) != motor[i].cmd) // Motor has not been set to target
            {
                int current = motorGet(i+1);
                int commanded = motor[i].cmd;
                float slew = motor[i].slewrate;
                int out = 0;
								// If slew is less than required delta-PWM, set commanded to output
                if (abs(commanded - current) < (slew * (millis() - motor[i]._lastUpdate)))
                    out = commanded;
								// Basic Idea: if we are speeding up too fast the wheels might slip so we need to control accel.
                else
                    out = (current + (int)(slew * (millis() - motor[i]._lastUpdate) * (commanded - current > 0 ? 1 : -1)));
								// Recalculates if truespeed is desired
                out = motor[i].recalculate(out);
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

/**
 * @brief Initializes the Motor Manager Task by creating the Motor Mutexes and starting the task.
 *        NOTE: MUST BE CALLED IN "INIT.C" TO USE SML
 */
void motorManagerInit()
{
    for (int i = 0; i < NUM_MOTORS; i++)
    {
        mutex[i] = mutexCreate();
        motor[i].recalculate = &defaultRecalculate;
    }
    motorManagerTaskHandle = taskCreate(_motorManagerTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_HIGHEST-1);
}
/**
 * @brief Kills the motor manager task, if it exists
 */
void motorManagerStop()
{
    if (motorManagerTaskHandle != NULL) // passing NULL kills current thread, so don't allow that to happen
        taskDelete(motorManagerTaskHandle);
}

/**
 * @brief Change the motor speed
 *
 * @param channel
 *        The port of the motor (1-10)
 *
 * @param set
 *        The PWM value of the motor to set it to. Will be checked and forced back into the bounds [-127,127]
 *
 * @param immediate
 *        Will change the speed of the motor immediately, bypassing the motor manager ramping if set to true.
 *
 * @returns Returns true if MotorSet was successful.
 */
bool smartMotorSet(int port, int commanded, bool immediate)
{
    if (port > 10 || port < 1)
        return false;
    if (commanded > 127)
        commanded = 127;
    else if (commanded < -127)
        commanded = -127;
    port--;
    if (immediate)
    {
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


/**
 * @brief Returns the normalized commanded speed of the motor
 *
 * @param channel
 *          The port of the motor [1,10]
 *
 * @returns Returns the cmd speed of the motor
 */
int smartMotorGet(int port)
{
    if (port > 10 || port < 1)
        return 0;
    port--;
    return motor[port].cmd * motor[port].inverted;
}
/**
 * @brief Configures a motor port with inversion, slew, and scaling
 *
 * @param channel
 *        The port of the motor [1,10]
 *
 * @param inverted
 *        If the motor port is inverted, then set to true (127 will become -127 and vice versa)
 *
 * @param slew
 *        The acceleration of the motor in dPWM/millisecond. DEFAULT_SLEW_RATE is available, which sets dPWM/millisecond to 0.75
 *
 * @param recalculate
 *        function pointer to the scaling function for the motor. The default function (NULL) doesn't affect the output at all.
 *        passing "trueSpeed" here activates true speed recalculation
 */
void smartMotorInit(int port, bool inverted, float slewrate, int (*recalculate)(int))
{
    if (port < 1 || port > 10)
        return;
    port--;
    motor[port].port = port + 1;
    motor[port].inverted = inverted ? -1 : 1;
    motor[port].slewrate = slewrate;
    if(!recalculate) {
      motor[port].recalculate = &defaultRecalculate;
    }
    else {
      motor[port].recalculate = recalculate;
    }
}
