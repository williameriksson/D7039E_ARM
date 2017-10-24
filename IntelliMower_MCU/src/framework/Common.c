/*
 * Common.c
 */

#include "Common.h"
#include "com/SPI.h"
#include "motors/Steering.h"
#include "sensors/UltraSonic.h"
#include "utils/containers/CircularBuffer.h"
#include "utils/containers/Queue.h"
#include "utils/containers/Stack.h"
#include "com/I2C.h"
#include "sensors/Accelerometer.h"

// private variables
volatile int isInitialized = 0;
volatile int quitProgram   = 0;

/*
 *  Master initialize for the whole program
 */
void CommonInit() {
	// place all inits here
//	InitSPI();
//	InitSteering();
	InitUltrasonic();
	isInitialized = 1;
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

	while( !quitProgram ) {
		// busy wait, let interrupt handlers take over
	}
}



