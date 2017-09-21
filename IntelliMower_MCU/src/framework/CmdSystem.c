/*
 * CmdSystem.c
 */

#include "utils/GPIO.h"
#include "framework/CmdSystem.h"
#include "motors/Steering.h"
#include "utils/convertions/intLib.h"
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

void InterpretCmds( uint8_t *rpiCmds, rpiCMD_t *newCMD , mCoords_t *mCoords, setMotor_t *setMotor ) {
//	*newCMD = rpiCmds[0];
//	setMotor->speed = (int8_t)rpiCmds[1];
//	testSpd = rpiCmds[1];
//	int32_t coords[2];
//	ByteArrToInt32(&rpiCmds[2], 8, coords);
//	testCoords[0] = coords[0];
//	testCoords[1] = coords[1];
//	mCoords->xpos = coords[0];
//	mCoords->ypos = coords[1];
}

/*
 *  Executes the RPI commands on the MCU
 *  FIXME! can a Internal interrupt or a Usonic interrupt, interrupt this?
 */
uint8_t RunCommand( uint8_t *rpiCmds ) {
	rpiCMD_t newCMD = rpiCmds[0];
	setMotor_t setMotor = { .speed = (int8_t)rpiCmds[1] };
	testSpd = setMotor.speed;
	int32_t coordinates[2] = {0, 0};
	// catch if no cmd was sent
	if( newCMD  == MCU_NULL ) return 0;

	// if control loop is activated a MCU_STOP is required all other cmd's are ignored
	if (newCMD  != MCU_STOP && inControlLoop  ) return 0;

	switch ( newCMD ) {
		case MCU_STOP :
			/*GpioEnable( GPIOA );
			GpioSetOutput( GPIOA, 5 );
			GpioSetPinHigh( GPIOA, 5 );*/ 			// set GPIOA 5 high
			// stop steering
			// stop control loop interrupts
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
			ByteArrToInt32(&rpiCmds[2], 8, coordinates);
			mCoords_t mCoords = { .xpos = coordinates[0], .ypos = coordinates[1] };
			inControlLoop = 1;
			// start internal interrupts that handle the control loop
			// FIXME! feed mCoords to control loop make an fifo in there
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

