/*
 * CmdSystem.c
 */


#include "framework/CmdSystem.h"


/*
 *  Executes the commands
 */
uint8_t RunCommand( const rpiCMD_t *rpiCMD ) {
	// catch if no cmd was sent for execution
	if( *rpiCMD == MCU_NULL ) return 0;

	switch ( *rpiCMD ) {
		case MCU_NULL :

		case MCU_STOP :

		case MCU_FORWARD :

		case MCU_BACK 	:

		case MCU_LEFT 	:

		case MCU_RIGHT 	:

		case MCU_MOVE 	:
		break;

	}
	return 1;
}
