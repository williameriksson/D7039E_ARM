/*
 *  Abstract functions and control loop for the motor functions and states
 */

#include <framework/Cmd.h>
#include "ControlLoop.h"

#include "motors/Steering.h"
#include "motors/MotorControl.h"

/*
 *
 */
void EvalObstacle () {
	// Enable interrupt again

}

/*
 *
 */
void IdleMode( rpiCMD_t *rpiCMD ) {
	// Enable interrupt again

}

/*
 *  Used for a busy wait in the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 */
void MowMode( ) {
	// Enable interrupt again

}
