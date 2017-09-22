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

#define CTRL9_XL_ADDR 0x18
#define CTRL1_XL_ADDR 0x10
#define INT1_CTRL_ADDR 0x0D
#define STATUS_REG_ADDR 0x1E
#define OUTX_L_G 0x22

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

/*
 *  In the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 *  FIXME! Change name
 */




uint8_t writeMsgQ[4] = {CTRL9_XL_ADDR, 0x38, CTRL1_XL_ADDR, 0x60};//, INT1_CTRL_ADDR, 0x01};
uint8_t readMsgQ[2] = {STATUS_REG_ADDR, OUTX_L_G};

void I2C1_EV_IRQHandler (void) {
	static volatile int i = 0;
	uint8_t data;

	uint16_t SR1 = I2C1->SR1;
	uint16_t SR2 = I2C1->SR2;

	if (SR1 & I2C_SR1_SB) {
		if (i <= 5) {
			I2C1->DR = 0b11010100;
		} else {
			I2C1->DR = 0b11010101;
		}

	}

//	if (SR1 & I2C_SR1_ADDR) {
//		int throw = I2C1->SR2;
//		//I2C1->DR = writeMsgQ[0];
////		i++;
//	}

	// http://www.robot-electronics.co.uk/i2c-tutorial
	if ((SR1 & I2C_SR1_BTF) || (SR1 & I2C_SR1_TXE)) {
		//int throw = I2C1->SR2;
		if (i <= 3) {
			I2C1->DR = writeMsgQ[i];
			I2C1->CR1 |= I2C_CR1_START;
		} /*else if (i == 6){
			I2C1->CR1 |= I2C_CR1_STOP;
		} else if (i == 4) {
			I2C1->CR1 |= I2C_CR1_START;
		}*/ else {
			if (SR2 & I2C_SR2_TRA) {
				I2C1->DR = readMsgQ[1];
				I2C1->CR1 |= I2C_CR1_START;
			} else {
				data = I2C1->DR;
			}
		}
		i++;
	}

//	data = I2C1->DR;
}

void Timer_Interrupt_For_ControlLoop( ) {


}
