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
	MOVE 	= 0x05,

} rpiCMD_t;



#endif /* FRAMEWORK_CMDSYSTEM_H_ */
