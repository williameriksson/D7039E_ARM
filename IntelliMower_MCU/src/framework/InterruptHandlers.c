/*
 * InterruptHandlers.c
 */
#include <stdio.h>


#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "../com/I2C.h"

#include "utils/GPIO.h"
#include "utils/Cobs.h"

#include "InterruptHandlers.h"
#include "CmdSystem.h"


// private variables

#define RECEIVE_BUFSIZE 80

int bufIndex = 0;
uint8_t rcvBuf[RECEIVE_BUFSIZE];
uint8_t testCmd = 0;

I2C_STATE CURRENT_I2C_STATE = STOP;
I2C_MASTER_ROLE CURRENT_I2C_ROLE = TRANSMITTER;

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

/*
 *  In the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 *  FIXME! Change name
 */



#define nrOfWriteMsgs 6
uint8_t writeMsgQ[nrOfWriteMsgs] = {CTRL9_XL_ADDR, 0x38, CTRL1_XL_ADDR, 0x60, INT1_CTRL_ADDR, 0x01};

#define nrOfReadMsgs 2
uint8_t readMsgQ[nrOfWriteMsgs] = {STATUS_REG_ADDR, OUTX_L_XL};


void I2C1_EV_IRQHandler (void) {

	switch (CURRENT_I2C_STATE) {

		case STOP:
			if ( I2C1->SR1 & I2C_SR1_SB ) {
				CURRENT_I2C_STATE = START;
			}
			break;

		case START:
			if (I2C1->SR1 & I2C_SR1_ADDR) {
				CURRENT_I2C_ROLE = (I2C1->SR2 & I2C_SR2_TRA) ? TRANSMITTER : RECEIVER;
				CURRENT_I2C_STATE = SAK_REG_ADDR;
			}
			break;

		case SAK_REG_ADDR:
			if ( I2C1->SR1 & I2C_SR1_SB ) {
				CURRENT_I2C_STATE = REPEATED_START;
			} else {
				CURRENT_I2C_STATE = READY_TO_WRITE;
			}
			break;

		case REPEATED_START:
			if ( I2C1->SR1 & I2C_SR1_ADDR ) {
				CURRENT_I2C_ROLE = (I2C1->SR2 & I2C_SR2_TRA) ? TRANSMITTER : RECEIVER;
			}

			if (I2C1->SR1 & I2C_SR1_RXNE) {
				CURRENT_I2C_STATE = RECEIVING;
			}
			break;

		case RECEIVING:
			if (!(I2C1->CR1 & I2C_CR1_ACK)) {
				CURRENT_I2C_STATE = READ_LAST_BYTE;
			}
			break;

		case READ_LAST_BYTE:
			CURRENT_I2C_STATE = STOP;
			break;

		case READY_TO_WRITE:
			CURRENT_I2C_STATE = STOP;
			break;

		default:
			break;
	}

	if (CURRENT_I2C_STATE == STOP && (SR1 & I2C_SR1_SB)) {
		CURRENT_I2C_STATE = START;
	}


}

//void I2C1_EV_IRQHandler (void) {
//	static volatile int i = 0;
//	static volatile int sentWriteMsgs = 0;
//	static volatile int sentReadMsgs = 0;
//	static volatile uint8_t transmitter = 1;
//	uint8_t data;
//
//
//	if (I2C1->SR1 & I2C_SR1_SB) {  // Start bit set, Send the slave address and R/W bit
//		I2C1->CR1 |= I2C_CR1_ACK;
//		if (sentWriteMsgs < nrOfWriteMsgs) {
//			I2C1->DR = SLAVEADDRWRITE;
//		}
//
//		else if (sentReadMsgs % 2 == 0) {
//			I2C1->DR = SLAVEADDRWRITE;
//		}
//
//		else {
//			I2C1->DR = SLAVEADDRREAD;
//		}
//
//	}
//
//	if (I2C1->SR1 & I2C_SR1_ADDR) {					// The slave address has been sent, clear the flag by reading SR2 also.
//		I2C1->CR1 &= ~I2C_CR1_ACK;					// Clear the ACK bit to generate No master ack event
//		transmitter = (I2C1->SR2 >> 2) & 1;
//		//transmitter = (I2C1->SR2 & I2C_SR2_TRA);	// The SR2 register should only be read when ADDR flag is set.
//													// Check the TRA bit in SR2 to determine if in trans. or rec. mode
//	}
//
//
//	if (((I2C1->SR1 & I2C_SR1_BTF) || (I2C1->SR1 & I2C_SR1_TXE)) && transmitter >= 1) { // If the byte transfer is completed and in transmitter mode
//
//
//		if (sentWriteMsgs < nrOfWriteMsgs && (i + 1) % 2 == 0) {
//			I2C1->CR1 |= I2C_CR1_STOP;		// Sub address and data has been sent, send stop sequence
//			for(int j = 0; j < 5000; j++);	// Ugly wait for a little while, how should this be changed?
//			I2C1->CR1 |= I2C_CR1_START;
//		}
//
////		else if (sentWriteMsgs >= nrOfWriteMsgs && 1) {
////
////		}
//
//		else if (sentWriteMsgs < nrOfWriteMsgs) {		// If transmitter and still got write bytes to send
//			I2C1->DR = writeMsgQ[sentWriteMsgs];		// Send the next write byte
//			sentWriteMsgs++;
//		}
//
//		else {
//			I2C1->DR = readMsgQ[1];			// Send read byte
//			sentReadMsgs++;
//			I2C1->CR1 |= I2C_CR1_START;
//		}
//		i++;
//
//
//
//	} else if (((I2C1->SR1 & I2C_SR1_BTF) || (I2C1->SR1 & I2C_SR1_RXNE)) && transmitter == 0) { // If the byte transfer is completed and in receiver mode
//		data = I2C1->DR;
//		I2C1->CR1 |= I2C_CR1_STOP;
//		for(int j = 0; j < 5000; j++);	// Ugly wait for a little while, how should this be changed?
//		I2C1->CR1 |= I2C_CR1_START;
//	} else {
////		I2C1->CR1 |= I2C_CR1_STOP;
//		I2C1->CR1 |= I2C_CR1_START;
////		sentReadMsgs++;
//	}
//
//
//}

void Timer_Interrupt_For_ControlLoop( ) {


}
