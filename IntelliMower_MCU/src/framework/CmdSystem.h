#ifndef FRAMEWORK_CMDSYSTEM_H_
#define FRAMEWORK_CMDSYSTEM_H_

#include <stdint.h>

#include "com/UART.h"
#include "framework/ControlLoop.h"
#include "framework/SafetyControl.h"
#include "motors/Steering.h"
#include "sensors/Gyroscope.h"
#include "sensors/Encoder.h"

#include "utils/GPIO.h"
#include "utils/convertions/intLib.h"
#include "utils/convertions/floatLib.h"
#include "utils/Geometry.h"
#include "utils/Cobs.h"

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
	MCU_POLL_DATA		= 0x0A,
	MCU_NULL			= 0x00,
} rpiCMD_t;



//
int RunCommand( uint8_t *rpiCmds );
void SendStats();


#endif /* FRAMEWORK_CMDSYSTEM_H_ */
