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


// private variables

#define RECEIVE_BUFSIZE 80

int bufIndex = 0;
uint8_t rcvBuf[RECEIVE_BUFSIZE];
uint8_t testVar = 0;

/*
 * FIXME! Change name
 */
void Usonic_InterruptHandler () {


}

/*
 *  Used for SPI commands from raspberry
 */
void SPI2_IRQHandler (void) {

	rpiCMD_t newCMD 	= MCU_NULL;
	mCoords_t mCoords 	= { .xpos = 0, .ypos = 0 };

	uint8_t ch = 0;

	if ( SPI2->SR & SPI_SR_RXNE ) {

		ch = SPI2->DR;					// read from spi reg (spi clears bit own its own)

		if( ch ) {
			rcvBuf[bufIndex] = ch;
			bufIndex++;
		} else {	// run after receiving stop bit from cobs
			rcvBuf[bufIndex] = ch;
			uint8_t decoded[bufIndex-1];
			UnStuffData( rcvBuf, bufIndex+1, decoded );

			newCMD  = (uint8_t)decoded[0];
			mCoords.xpos = decoded[1];
			mCoords.ypos = decoded[2];
			testVar = decoded[0];

			// execute RPI command on MCU
			// coordinates will come after the move cmd in same transmission
			if ( !RunCommand( newCMD, &mCoords ) ) {
				//catch errors here
			}
			bufIndex = 0;
		}
	}

}

/*
 *  In the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 *  FIXME! Change name
 */
void Timer_Interrupt_For_ControlLoop( ) {


}
