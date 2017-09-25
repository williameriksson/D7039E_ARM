/*
 * InterruptHandlers.c
 */
#include <stdio.h>


#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"

#include "utils/GPIO.h"
#include "utils/Cobs.h"

#include "InterruptHandlers.h"
#include "CmdSystem.h"
#include "framework/ControlLoop.h"


// private variables

#define RECEIVE_BUFSIZE 128

int bufIndex = 0;
uint8_t rcvBuf[RECEIVE_BUFSIZE];
uint8_t testCmd = 0;

/*
 * FIXME! Change name
 */
void Usonic_InterruptHandler () {


}

/*
 *  Used for SPI commands from raspberry
 */
void SPI2_IRQHandler (void) {

	uint8_t ch = 0;

	if ( SPI2->SR & SPI_SR_RXNE ) {

		ch = SPI2->DR;					// read from SPI reg (SPI clears bit own its own in MCU)

		if( ch ) {
			rcvBuf[bufIndex] = ch;
			bufIndex++;
		} else {	// run after receiving stop bit from cobs
			rcvBuf[bufIndex] = ch;
			uint8_t rpiCmds[bufIndex-1];
			UnStuffData( rcvBuf, bufIndex+1, rpiCmds ); // decode received commands

			testCmd = rpiCmds[0];

			// execute decoded RPI commands on MCU
			// coordinates will come after the move cmd in same transmission
			if ( !RunCommand( rpiCmds ) ) {
				//catch errors here
			}
			bufIndex = 0;
		}
	}

}

//currently assigned by ControlLoop.c as internal interrupt for PID
void TIM1_BRK_TIM9_IRQHandler() {
	LoopController();
	TIM9->SR &= ~TIM_SR_TIF;
}


