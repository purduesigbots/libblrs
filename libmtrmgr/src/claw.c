#include "claw.h"
#include "main.h"
#include "ports.h"

void clawSet(int speed) {
	// need to invert claw direction
	blrsMotorSet(CLAW_MOTOR, speed, false);
}

void clawInit() {
	blrsMotorInit(CLAW_MOTOR, true, 2.0, NULL);
}
