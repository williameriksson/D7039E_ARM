/*
 * initSPI.c
 *
 *  Created on: 1 sep. 2017
 *      Author: rjons
 */
#include "SPI.h"
#include "framework/CmdSystem.h"
#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "utils/GPIO.h"
#include "utils/Cobs.h"

// private variables
//#define RECEIVE_BUFSIZE 128
//#define INIT_BUFSIZE 2
//
//int bufIndex = 0;
//uint8_t rcvBuf[RECEIVE_BUFSIZE];
//uint8_t testCmd = 0;

void InitSPI () {

	__disable_irq();

	//Enables clock for GPIOC and SPI2 interface
	GpioEnable(GPIOB);
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

	//Set AF05 on GPIOB pin 12-15
	GpioSetAF(GPIOB, 12, 5);
	GpioSetAF(GPIOB, 13, 5);
	GpioSetAF(GPIOB, 14, 5);
	GpioSetAF(GPIOB, 15, 5);

	//Sets Output speed to high for PB12-15
	GpioSetOutSpeed(GPIOB, 12, HIGH);
	GpioSetOutSpeed(GPIOB, 13, HIGH);
	GpioSetOutSpeed(GPIOB, 14, HIGH);
	GpioSetOutSpeed(GPIOB, 15, HIGH);

	//SPI settings. RX_ONLY enabled
	//SPI2->CR1 |= SPI_CR1_RXONLY;

	//Enables recieve not empty interrupt
	SPI2->CR2 |= SPI_CR2_RXNEIE;


	//Enables SPI, This MUST happen after SPI configuration
	SPI2->CR1 |= SPI_CR1_SPE;


	NVIC_EnableIRQ(SPI2_IRQn);
	NVIC_SetPriority(SPI2_IRQn, 36);
	__enable_irq();

}



//void SPI2_IRQHandler (void) {
//
//	uint8_t ch = 0;
//
//	if ( SPI2->SR & SPI_SR_RXNE ) {
//
//		ch = SPI2->DR;					// read from data reg, interrupt bit cleared by hardware on read.
//
//		if( ch ) {
//			rcvBuf[bufIndex] = ch;
//			bufIndex++;
//		} else {	// run after receiving stop bit from cobs
//			rcvBuf[bufIndex] = ch;
//			uint8_t rpiCmds[bufIndex-1];
//			UnStuffData( rcvBuf, bufIndex+1, rpiCmds ); // decode received commands
//
//			testCmd = rpiCmds[0];
//
//			// execute decoded RPI commands on MCU
//			// coordinates will come after the move cmd in same transmission
//			if ( !RunCommand( rpiCmds ) ) {
//				//catch errors here
//			}
//			bufIndex = 0;
//		}
//	}
//
//}



