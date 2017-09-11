/*
 * Common.c
 */
#include <stdint.h>
#include "Common.h"

#include "CmdSystem.h"
#include "ControlLoop.h"
#include "KeyInput.h"

// private variables
static uint8_t isInitialized = 0x00;
static uint8_t quitProgram	 = 0x00;

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
	while( !exitProgram ){

	}
}
