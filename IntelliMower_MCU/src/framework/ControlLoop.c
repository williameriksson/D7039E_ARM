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
void SPI2_IRQHandler (void) {

	uint8_t rpiCMD;

	if (SPI2->SR & SPI_SR_RXNE) {
		GPIOA->ODR |= GPIO_ODR_ODR_5;
		rpiCMD = SPI2->DR;
	}

	switch ( rpiCMD ) {

	case STOP :

	case FORWARD :

	case BACK 	:

	case LEFT 	:

	case RIGHT 	:

	case MOVE 	:
	break;

	}

}

/*
 *  Used for a busy wait in the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 */
void MowMode( ) {
	// Enable interrupt again

}
