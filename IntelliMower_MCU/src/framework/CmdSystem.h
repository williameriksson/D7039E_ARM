/*
 * Cmd.h
 */

#ifndef FRAMEWORK_CMDSYSTEM_H_
#define FRAMEWORK_CMDSYSTEM_H_


typedef enum {
	STOP 	= 0x00,
	FORWARD = 0x01,
	BACK 	= 0x02,
	LEFT 	= 0x03,
	RIGHT 	= 0x04,
	MOVE 	= 0x05,			// goes into the control loop and listens to feed from rpi
} rpiCMD_t;


void RunCommand(const rpi_t rpiCMD );


#endif /* FRAMEWORK_CMDSYSTEM_H_ */
