/*
 * CmdSystem.c
 */

#include "utils/GPIO.h"
#include "framework/CmdSystem.h"
#include "motors/Steering.h"
#include "utils/convertions/intLib.h"
#include "utils/Geometry.h"
#include "framework/ControlLoop.h"


// private variables
static volatile uint8_t inControlLoop = 0x00;

int8_t testSpd = 0;
int32_t testCoords[2];
//
typedef struct {
	int32_t xpos;
	int32_t ypos;
}mCoords_t ;


//
typedef struct {
	int8_t speed;
} setMotor_t;

void fetchData(uint8_t *cmdArray) {

}

/*
 *  Executes the RPI commands on the MCU
 *  FIXME! can a Internal interrupt or a Usonic interrupt, interrupt this?
 */
uint8_t RunCommand( uint8_t *rpiCmds ) {
	rpiCMD_t newCMD = rpiCmds[0];
	setMotor_t setMotor = { .speed = (int8_t)rpiCmds[1] };
	testSpd = setMotor.speed;
	int32_t coordinates[4] = {0, 0, 0, 0};
	// catch if no cmd was sent
	if( newCMD  == MCU_NULL ) return 0;

	// if control loop is activated a MCU_STOP is required all other cmd's are ignored
	if (newCMD  != MCU_STOP && inControlLoop && newCMD != MCU_FEED  ) return 0;

	switch ( newCMD ) {
		case MCU_STOP :
			StopController();
			DriveForward(0);
			inControlLoop = 0;
			break;
		case MCU_FORWARD :
			// drive forward
			DriveForward( setMotor.speed );
			break;
		case MCU_REVERSE :
			// drive backwards
			DriveForward( setMotor.speed );
			break;
		case MCU_LEFT :
			// rotate left
			RotateLeft( setMotor.speed );
			break;
		case MCU_RIGHT :
			// rotate right
			RotateRight( setMotor.speed );
			break;
		case MCU_MOVE :
			ByteArrToInt32(&rpiCmds[2], 16, coordinates);
			inControlLoop = 1;
			// start internal interrupts that handle the control loop
			// FIXME! feed mCoords to control loop make an fifo in there
			Point currentPos = { .x = coordinates[0], .y = coordinates[1]};
			Point target = { .x = coordinates[2], .y = coordinates[3]};
			DriveForward(setMotor.speed);
			InitControlLoop(&currentPos, &target, setMotor.speed);
			break;
		case MCU_FEED :
			ByteArrToInt32(&rpiCmds[1], 8, coordinates);
			Point newPos = { .x = coordinates[0], .y = coordinates[1] };
			testCoords[0] = newPos.x;
			testCoords[1] = newPos.y;
			UpdatePIDValue(&newPos);
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

