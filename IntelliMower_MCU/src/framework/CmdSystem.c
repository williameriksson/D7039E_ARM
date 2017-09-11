/*
 * CmdSystem.c
 */


#include "framework/CmdSystem.h"


// private variables
static uint8_t inControlLoop = 0x00;

/*
 *  Executes the RPI commands on the MCU
 *  FIXME! can a Internal interrupt or a Usonic interrupt, interrupt this?
 */

uint8_t RunCommand( const rpiCMD_t *rpiCMD ) {
	// catch if no cmd was sent
	if( *rpiCMD == MCU_NULL ) return 0;

	// if control loop is activated a MCU_STOP is required all other cmd's are ignored
	if (*rpiCMD != MCU_STOP && inControlLoop  ) return 0;

	switch ( *rpiCMD ) {
		case MCU_STOP :
			// stop steering
			// stop control loop interrupts
			inControlLoop = 0;
			break;
		case MCU_FORWARD :
			// steer forward
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
			// start control loop
			inControlLoop = 1;
			break;
		case MCU_NULL :
			// never gets here
			break;
	}
	return 1;
}
