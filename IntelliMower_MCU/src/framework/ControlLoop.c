/*
 *  Abstract functions and control loop for the motor functions and states
 */

#include "ControlLoop.h"

#include "CmdSystem.h"
#include "motors/Steering.h"
#include "motors/MotorControl.h"

/*
 *
 */
void EvalObstacle () {

	// Enable interrupt again

	while(1) {

	}
}

/*
 *
 */
void IdleMode( rpiCMD_t *rpiCMD ) {

	// Enable interrupt again

	while(1){




	}


}

/*
 *  Used for a busy wait in the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 */
void MowMode( ) {
	// Enable interrupt again
	while(1){




	}
}
