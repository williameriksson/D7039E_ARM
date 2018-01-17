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
//	InitSPI();
	InitBladeControl();
	InitEncoders();
	StartBlades();
//	InitI2C();
//	InitAccMag();
//	InitGyro();
//	gyroCalibrated = 1;
	InitUltrasonic();
//	InitSafetyControl();
	isInitialized = 1;
//	InitUART();
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

int nextPoint;
Point pointQueue[] = {
		{.x = 100.0, .y = 0.0},
		{.x = 100.0, .y = 100.0},
		{.x = 0.0, .y = 100.0},
		{.x = 0.0, .y = 0.0}
};

Point pointQueueTwo[] = {
		{.x = 100.0, .y = 0.0},
		{.x = 100.0, .y = 100.0},
		{.x = 0.0, .y = 100.0},
		{.x = 0.0, .y = 200.0},
		{.x = 200.0, .y = 200.0},
		{.x = 0.0, .y = 0.0}
};



/* for spoofing input from the PI. */
void MakeMoveCMD(Point target, uint8_t speed, uint8_t *cmdString) {
	cmdString[0] = 0x06; //move cmd
	cmdString[1] = speed;
	double coords[4];
	coords[0] = 0.0;
	coords[1] = 0.0;
	coords[2] = target.x;
	coords[3] = target.y;
	DoubleToByteArray(coords, 4, cmdString+2);
}

void CheckState(Point *pointList, int length) {
	if(_state == IDLE && length == nextPoint) {
		//reached final point and is idle.
	}
	else if(_state == IDLE) {
		uint8_t moveCMD[34];
		MakeMoveCMD(pointQueue[nextPoint], 200, moveCMD);
		nextPoint++;
		RunCommand(moveCMD);
	}
	else if(_state == DRIVING) {
		//is driving, just wait.
	}
	else if(_state == TURNING) {
		//is turning, just wait.
	}
}

void CommonFrame() {
	state = 0;
	nextPoint = 0;

	for(int i = 0; i < 10000000; i++) {

	}

	/* motors with direction pin below */
//	TimerSetPWM(TIM2, 1, 500, 1000);
//	TimerSetPWM(TIM2, 3, 500, 1000);
//	GpioEnable(GPIOC);
//	GpioSetOutput(GPIOC, 13);
//	GpioSetOutput(GPIOC, 14);
//	GpioSetPinHigh(GPIOC, 13);
//	GpioSetPinHigh(GPIOC, 14);
	//end
//	DriveForward(-4000);
	while( !quitProgram ) {
//		state++;
//		ReadAccMagData();
//		ReadGyroData();
		//demos below
//		state = DemoPointToPoint(state, -3000);
//		DriveForward(-5000);

		DrivingHandler(&currentPosition);
		SteeringHandler(posAngle); //in case a steering procedure is ongoing.
		//UpdatePIDValue(&curP);
		CheckState(pointQueue, (sizeof(pointQueue) / sizeof(pointQueue[0])));
	}
}




