/*
 *  Abstract functions and control loop for the motor functions and states
 */

#include "ControlLoop.h"


#include "motors/Steering.h"
#include "motors/MotorControl.h"

/*
 *
 */
void EvalObstacle () {

}

/*
 *
 */
void IdleMode( MSG) {

	// Enable interrupt

	while(1){




	}


}

/*
 *  Used for a busy wait in the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 */
void MowMode( ) {

	while(1){




	}
}
