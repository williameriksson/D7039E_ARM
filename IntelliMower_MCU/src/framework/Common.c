#include "Common.h"
// private variables
volatile int isInitialized = 0;
volatile int quitProgram   = 0;

/*
 *  Master initialize for the whole program
 */
void CommonInit() {
	// place all inits here
	InitSteering();
	for(int i; i < 10000000; i++) {

	}
	InitSPI();
	InitEncoders();
	InitI2C();
	InitAccMag();
	InitGyro();
	gyroCalibrated = 1;
	InitUltrasonic();
//	InitSafetyControl();
	isInitialized = 1;
	InitUART();
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
int state;

void CommonFrame() {
	state = 0;
//	uint8_t spiTest = 0;
//	Point currentPos = { .x = 0.0f, .y = 0.0f};
//	Point target = { .x = 0.0f, .y = 100.0f};

//	gyro.yaw = 0.0f;
//	DriveForward(-20);
//	InitControlLoop(&currentPos, &target, -20);
//	RotateLeft(2000);
//	Point start = {.x = 0.0, .y = 0.0};
//	Point target = {.x = 0.0, .y = 200.0};

//	InitControlLoop(&start, &target, -3000);

	while( !quitProgram ) {
//		state++;
//		USART6->DR = 0x1; //for testing UART
		ReadAccMagData();
		ReadGyroData();

		//demos below
//		state = DemoCurve(state, -3000, 200.0);
//		state = DemoSquare(state, -3000);
//		state = DemoRotate(state, -3000, 0);
//		state = DemoEncoderNav(state, -3000);
//		state = DemoAvoidance(state, -3000);

//		state = DemoPointToPoint(state, -3000);
		DrivingHandler(&currentPosition);
		SteeringHandler(posAngle); //in case a steering procedure is ongoing.
//		UpdatePIDValue(&curP);
	}
}



