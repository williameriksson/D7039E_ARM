/*
 * CmdSystem.c
 */

#include "utils/GPIO.h"
#include "framework/CmdSystem.h"


// private variables
static volatile uint8_t inControlLoop = 0x00;


/*
 *  Executes the RPI commands on the MCU
 *  FIXME! can a Internal interrupt or a Usonic interrupt, interrupt this?
 */
uint8_t RunCommand( rpiCMD_t rpiCMD, mCoords_t *mCoords) {

	// catch if no cmd was sent
	if( rpiCMD == MCU_NULL ) return 0;

	// if control loop is activated a MCU_STOP is required all other cmd's are ignored
	if (rpiCMD != MCU_STOP && inControlLoop  ) return 0;

	switch ( rpiCMD ) {

		case MCU_STOP :
			/*GpioEnable( GPIOA );
			GpioSetOutput( GPIOA, 5 );
			GpioSetPinHigh( GPIOA, 5 );*/ 			// set GPIOA 5 high
			// stop steering
			// stop control loop interrupts
			inControlLoop = 0;
			break;
		case MCU_FORWARD :
			// steer forward
			// Light on used for debugging
			break;
		case MCU_REVERSE :
			// steer backwards
			break;
		case MCU_LEFT :
			// steer left
			break;
		case MCU_RIGHT :
			// steer right
			break;
		case MCU_MOVE :

			// start internal interrupts that handle the control loop
			// FIXME! feed mCoords to control loop make an fifo in there
			inControlLoop = 1;
			break;
		case MCU_NULL :
			// never gets here
			break;
		default :
			return 0;
			break;
	}
	return 1;
}

