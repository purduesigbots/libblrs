## BLRSLib by Purdue SIGBots

Helpful utilities for making the most of PROS.

### libbtns: Buttons Library
This library makes it easier to interact with the Joystick and LCD buttons.

A full description of its features can be found in its header file, "buttons.h"

### libfbc: Feedback Controller Library
This library contains abstracted feedback controllers, making it easier to use PID, TBH, and other control algorithms.

PID is currently the only algorithm supported in this library (fbc_pid).

Similarly, a full description of its features can be found in its header files, "fbc.h", "fbc_util.h", and "fbc_pid.h"

### libmtrmgr: Motor Manager Library
This library allows the user to easily integrate slewing, inversion, and scaling to the motor output. In many cases, the slewing and scaling can improve the motor's response to feedback control and reduce the likelihood of PTC trips.

As with the previous libraries, a full description of its features can be found in its header file, "mtrmgr.h"
