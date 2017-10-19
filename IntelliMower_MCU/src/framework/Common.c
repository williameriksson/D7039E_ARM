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

// private variables
volatile int isInitialized = 0;
volatile int quitProgram   = 0;

/*
 *  Master initialize for the whole program
 */
void CommonInit() {
	// place all inits here
	InitSPI();
	InitSteering();
//	InitUltrasonic();
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
//static int kek;

void CommonFrame() {

	while( !quitProgram ) {
		// busy wait, let interrupt handlers take over
//		kek = TIM3->CCR1;
	}
}

void TIM2_IRQHandler() {
//	LoopController();
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2->SR &= ~TIM_SR_CC1IF;
//	GPIOA->ODR ^= 0x32;
	GpioSetPinToggle(GPIOA, 5);
}


