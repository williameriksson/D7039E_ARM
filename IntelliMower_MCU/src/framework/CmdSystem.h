/*
 * Cmd.h
 */

#ifndef FRAMEWORK_CMDSYSTEM_H_
#define FRAMEWORK_CMDSYSTEM_H_

#include <stdint.h>
#include "utils/GPIO.h"
#include "framework/ControlLoop.h"
#include "motors/Steering.h"
#include "sensors/Gyroscope.h"
#include "sensors/Encoder.h"
#include "utils/convertions/intLib.h"
#include "utils/Geometry.h"

//
typedef enum {
	MCU_STOP 			= 0x01,			// Stop currently running command and go into idle
	MCU_FORWARD 		= 0x02,
	MCU_REVERSE 		= 0x03,
	MCU_LEFT 			= 0x04,
	MCU_RIGHT 			= 0x05,
	MCU_MOVE 			= 0x06,			// goes into the control loop and listens to feed from rpi
	MCU_FEED			= 0x07,
	MCU_START_BLADES 	= 0x08,
	MCU_STOP_BLADES 	= 0x09,
	MCU_NULL			= 0x00,
} rpiCMD_t;



//
uint8_t RunCommand( uint8_t *rpiCmds );



#endif /* FRAMEWORK_CMDSYSTEM_H_ */
