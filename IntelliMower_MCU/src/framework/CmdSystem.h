/*
 * Cmd.h
 */

#ifndef FRAMEWORK_CMDSYSTEM_H_
#define FRAMEWORK_CMDSYSTEM_H_

#include <stdint.h>

//
typedef enum {
	MCU_STOP 	= 0x01,			// Stop currently running command and go into idle
	MCU_FORWARD = 0x02,
	MCU_REVERSE = 0x03,
	MCU_LEFT 	= 0x04,
	MCU_RIGHT 	= 0x05,
	MCU_MOVE 	= 0x06,			// goes into the control loop and listens to feed from rpi
	MCU_NULL	= 0x00,
} rpiCMD_t;



//
uint8_t RunCommand( uint8_t *rpiCmds );



#endif /* FRAMEWORK_CMDSYSTEM_H_ */
