/*
 * Common.c
 */
#include <stdint.h>
#include "Common.h"


// private variables
static volatile uint8_t isInitialized = 0;
static volatile uint8_t quitProgram   = 0;

/*
 *  Master initialize for the whole program
 */
void CommonInit() {
	// place all inits here

	isInitialized = 1;
}

/*
 *
 */
uint8_t CommonIsInitialized() {
	return isInitialized;
}

/*
 *
 */
void CommonQuit() {
	quitProgram = 1;
}

/*
 * "main"
 */
void CommonFrame() {
	while( !quitProgram ){
		// busy wait, let interrupt handlers take over
	}
}


