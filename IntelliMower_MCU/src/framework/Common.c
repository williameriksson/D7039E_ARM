/*
 * Common.c
 */
#include "Common.h"
// private variables
volatile int isInitialized = 0;
volatile int quitProgram   = 0;

/*
 *  Master initialize for the whole program
 */
void CommonInit() {
	// place all inits here
//	InitSPI();
	InitSteering();
	InitEncoders();
	InitI2C();
	InitAccMag();
	InitGyro();
//	InitUltrasonic();
	InitSafetyControl();
	isInitialized = 1;
	gyroCalibrated = 1;
//	initUART();
}

/*
 *
 */
int CommonIsInitialized() {
	return isInitialized;
}
/*
 *
 */
void CommonQuit() {
	//quitProgram = 1;
}


/*
 * "main"
 */

void CommonFrame() {
	int state = 0;
//	uint8_t spiTest = 0;
//	Point currentPos = { .x = 0.0f, .y = 0.0f};
//	Point target = { .x = 0.0f, .y = 100.0f};

//	gyro.yaw = 0.0f;
//	DriveForward(-20);
//	InitControlLoop(&currentPos, &target, -20);
	RotateLeft(2000);
	while( !quitProgram ) {

//		USART6->DR = 0x1; //for testing UART
		ReadAccMagData();
		ReadGyroData();

		//demos below
//		state = DemoCurve(state, -30, 200.0);
//		state = DemoSquare(state, -20);
//		state = DemoRotate(state, -30, 0);
//		state = DemoEncoderNav(state, -3000);
//		state = DemoAvoidance(state, -30);

//		SteeringHandler(posAngle); //in case a steering procedure is ongoing.
	}
}



