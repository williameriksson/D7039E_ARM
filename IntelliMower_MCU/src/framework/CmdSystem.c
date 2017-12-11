/*
 * CmdSystem.c
 */

#include "CmdSystem.h"

#define RECEIVE_BUFSIZE 128
#define INIT_BUFSIZE 2

// private variables
static volatile uint8_t inControlLoop = 0x00;
int firstMoveDone = 0;
Point moveQueue[3];
int stopReceived;
float turnDesired;

typedef struct {
	int8_t speed;
} setMotor_t;

int bufIndex = 0;
uint8_t rcvBuf[RECEIVE_BUFSIZE];

int spookCount = 0;

/*
 *  Executes the RPI commands on the MCU
 */
int RunCommand( uint8_t *rpiCmds ) {
	rpiCMD_t newCMD = rpiCmds[0]; //first byte is Command (message) type.
	setMotor_t setMotor = { .speed = (int8_t)rpiCmds[1] };
	double coordinates[4] = {0.0, 0.0, 0.0, 0.0};

	// catch if no cmd was sent
	if( newCMD  == MCU_NULL ) return 0;

	/* if control loop is activated a MCU_STOP is required all other cmd's are ignored
	 * except MCU_FEED or POLL_DATA */
	if (newCMD  != MCU_STOP && inControlLoop && newCMD != MCU_FEED && newCMD != MCU_POLL_DATA  ) return 0;

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
			// and start moving towards point.
			ByteArrayToDouble(&rpiCmds[2], 32, coordinates);
			inControlLoop = 0;
			Point target = { .x = coordinates[2], .y = coordinates[3]};
			double posAngleDiff = posAngle - prevPosAngle;

			if(firstMoveDone == 0) { //is this first move?
				spookCount++;
				moveQueue[0].x = 0.0; // initial pos
				moveQueue[0].y = 0.0;
				moveQueue[1].x = 0.0;
				moveQueue[1].y = 0.0;
				moveQueue[2].x = target.x; //target pos
				moveQueue[2].y = target.y;
//				DriveForward((-10)*setMotor.speed);
//				SetDriveSpeed((-10)*setMotor.speed);
				DriveForward(-3000);
				SetDriveSpeed(-3000);
				SetDriveTarget(moveQueue[2]);
				firstMoveDone = 1;
				_state = DRIVING;
			}
			else {
				spookCount++;
				moveQueue[0].x = moveQueue[1].x;
				moveQueue[0].y = moveQueue[1].y;

				moveQueue[1].x = currentPosition.x;
				moveQueue[1].y = currentPosition.y;

				moveQueue[2].x = target.x; //target pos
				moveQueue[2].y = target.y;

//				SetDriveSpeed((-10)*setMotor.speed);
				SetDriveSpeed(-3000);
				SetDriveTarget(moveQueue[2]);
				float anglesToTurn = GetTurnAngle(moveQueue[0], moveQueue[1], moveQueue[2]);
//				gyro.yaw = 0.0;
				turnDesired = anglesToTurn - posAngleDiff;
				RotateDegrees(turnDesired, posAngle);
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

		case MCU_POLL_DATA :
			SendStats();
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

/*  Pushes data to the R.pi (over UART)  */
void SendStats() {
	double doubleData[9];
	uint8_t sendData[78]; //9 doubles, 6 chars
	uint8_t cobsData[80]; //COBS has 2 byte overhead.

	sendData[0] = MCU_POLL_DATA-3	;	// message type
	sendData[1] = BumperFront;		// bumper sensors in front triggered
	sendData[2] = BumperBack;		// bumper sensors in back triggered
	sendData[3] = 0;				// TODO: cutting blade status (spinning or not)
	sendData[4] = 0;				// TODO: emergency stop hit
	sendData[5] = LiftSensor;		// lift sensor triggered

	doubleData[0] = (double)GetLeftSpeed();
	doubleData[1] = (double)GetRightSpeed();
	doubleData[2] = distanceSensors.distanceOuterRight; //ultrasonic 1
	doubleData[3] = distanceSensors.distanceInnerRight; //ultrasonic 2
	doubleData[4] = distanceSensors.distanceInnerLeft; //ultrasonic 3
	doubleData[5] = distanceSensors.distanceOuterLeft; //ultrasonic 4
	doubleData[6] = currentPosition.x; //xcoord
	doubleData[7] = currentPosition.y; //ycoord
	doubleData[8] = 0; //heading

	DoubleToByteArray(doubleData, 9, sendData+6);
	StuffData(sendData, 78, cobsData);
	SendData(cobsData, 80);
}

void USART6_IRQHandler (void) {
	uint8_t ch;
	if(USART6->SR & USART_SR_RXNE) {
		ch = (uint8_t) USART6->DR;
		if( ch ) {
			rcvBuf[bufIndex] = ch;
			bufIndex++;
		}
		else {	// run after receiving stop bit from cobs
			rcvBuf[bufIndex] = ch;
			uint8_t rpiCmds[bufIndex-1];
			UnStuffData( rcvBuf, bufIndex+1, rpiCmds ); // decode received commands

			// execute decoded RPI commands on MCU
			// coordinates will come after the move cmd in same transmission
			if ( !RunCommand( rpiCmds ) ) {
				//catch errors here
			}
			bufIndex = 0;
		}
	}
}

