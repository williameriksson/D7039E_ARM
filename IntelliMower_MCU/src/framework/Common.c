/*
 * Common.c
 */

#include "Common.h"
#include "com/SPI.h"
#include "motors/Steering.h"
#include "utils/containers/CircularBuffer.h"
#include "utils/containers/Queue.h"
#include "utils/containers/Stack.h"

// private variables
volatile int isInitialized = 0;
volatile int quitProgram   = 0;

/*
 *  Master initialize for the whole program
 */
void CommonInit() {
	// place all inits here
	InitSPI1();
	//InitSteering();
	//InitI2C();
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


