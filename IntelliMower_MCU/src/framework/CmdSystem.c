/*
 * CmdSystem.c
 */

#include "CmdSystem.h"

// private variables
static volatile uint8_t inControlLoop = 0x00;
int firstMoveDone = 0;
Point moveQueue[3];
int stopReceived;
float turnDesired;

typedef struct {
	int8_t speed;
} setMotor_t;

/*
 *  Executes the RPI commands on the MCU
 */
uint8_t RunCommand( uint8_t *rpiCmds ) {
	rpiCMD_t newCMD = rpiCmds[0];
	setMotor_t setMotor = { .speed = (int8_t)rpiCmds[1] };
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
			_state = IDLE;
			stopReceived++;
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
			// a move instruction has been recieved, decide how much to turn
			// and start controlloop.
			ByteArrToInt32(&rpiCmds[2], 16, coordinates);
			inControlLoop = 1;
			// start internal interrupts that handle the control loop
			Point currentPos = { .x = coordinates[0], .y = coordinates[1]};
			Point target = { .x = coordinates[2], .y = coordinates[3]};

			if(firstMoveDone == 0) { //is this first move?
				moveQueue[0].x = 0.0; // initial pos
				moveQueue[0].y = 0.0;
				moveQueue[1].x = 0.0;
				moveQueue[1].y = 0.0;
				moveQueue[2].x = target.x; //target pos
				moveQueue[2].y = target.y;
				DriveForward(-3000);
				SetDriveTarget(moveQueue[2]);
				firstMoveDone = 1;
				_state = DRIVING;
			}
			else {
				moveQueue[0].x = moveQueue[1].x;
				moveQueue[0].y = moveQueue[1].y;

				moveQueue[1].x = currentPosition.x;
				moveQueue[1].y = currentPosition.y;

				moveQueue[2].x = target.x; //target pos
				moveQueue[2].y = target.y;

				SetDriveTarget(moveQueue[2]);
				float anglesToTurn = GetTurnAngle(moveQueue[0], moveQueue[1], moveQueue[2]);
//				gyro.yaw = 0.0;
				turnDesired = anglesToTurn;
				RotateDegrees(anglesToTurn, posAngle);
			}
//			gyro.yaw = 0.0f;
//			DriveForward(setMotor.speed);
//			InitControlLoop(&currentPos, &target, setMotor.speed); //starts controlloop to keep on line
			break;
		case MCU_FEED :
			ByteArrToInt32(&rpiCmds[1], 8, coordinates);
			Point newPos = { .x = coordinates[0], .y = coordinates[1] };
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

