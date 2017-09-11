/*
 * Common.c
 */
#include <stdint.h>
#include "Common.h"

#include "CmdSystem.h"
#include "ControlLoop.h"
#include "KeyInput.h"


// private variables
static uint8_t isInitialized;
static uint8_t quitProgram;

/*
 *  Master initialize for the whole program
 */
void CommonInit() {

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


