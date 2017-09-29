/*
 * initSPI.c
 *
 *  Created on: 1 sep. 2017
 *      Author: rjons
 */
#include "SPI.h"
#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "utils/GPIO.h"

// private variables
#define RECEIVE_BUFSIZE 128
#define TRANSMIT_BUFSIZE 2
#define INIT_BUFSIZE 2

#define READ_Y_L 0x2A + 0x80
#define READ_Y_H 0x2B + 0x80

int bufIndex = 0;
uint8_t rcvBuf[RECEIVE_BUFSIZE];
uint8_t testCmd = 0;


int accRcvBufIndex = 0;
uint8_t accRcvBuf[RECEIVE_BUFSIZE];


int accInitBufIndex = 0;
uint8_t accInitBuf[INIT_BUFSIZE] = {0x20, 0x67};


int accTraBufIndex = 0;
uint8_t accTraBuf[TRANSMIT_BUFSIZE] = {READ_Y_L, READ_Y_H};

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

//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14;
//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;

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

void InitSPI1() {
	__disable_irq();

	GpioEnable(GPIOA);
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	GpioSetOutput(GPIOA, 4);
	GpioSetPinHigh(GPIOA, 4);

	//Set AF05 on GPIOB pin 12-15
	//GpioSetAF(GPIOA, 4, 5);
	GpioSetAF(GPIOA, 5, 5);
	GpioSetAF(GPIOA, 6, 5);
	GpioSetAF(GPIOA, 7, 5);

	GpioSetOutSpeed(GPIOA, 4, HIGH);
	GpioSetOutSpeed(GPIOA, 5, HIGH);
	GpioSetOutSpeed(GPIOA, 6, HIGH);
	GpioSetOutSpeed(GPIOA, 7, HIGH);

//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14;
//	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;

	//SPI settings. RX_ONLY enabled
	//SPI2->CR1 |= SPI_CR1_RXONLY;

	//SPI1->CR1 |= 0x08; // Baud rate fpclk / 4
	//SPI1->CR1 |= 0x10; // Baud rate fpclk / 8
	SPI1->CR1 |= 0x18; // Baud rate fpclk / 16
	//SPI1->CR1 |= 0x30; // Baud rate fpclk / 128
	//SPI1->CR1 |= 0x38; // Baud rate fpclk / 256
	//SPI1->CR1 |= SPI_CR1_CPOL;

	SPI1->CR2 |= SPI_CR2_TXEIE;

	SPI1->CR1 |= SPI_CR1_MSTR;

	SPI1->CR1 |= SPI_CR1_SSM;
	SPI1->CR1 |= SPI_CR1_SSI;

	//Enables recieve not empty interrupt
	SPI1->CR2 |= SPI_CR2_RXNEIE;

	//Enables SPI, This MUST happen after SPI configuration
	SPI1->CR1 |= SPI_CR1_SPE;




	NVIC_EnableIRQ(SPI1_IRQn);
	NVIC_SetPriority(SPI1_IRQn, 36);
	__enable_irq();

	GpioSetPinLow(GPIOA, 4);

//	while (1) {
//
//		//if (SPI1->SR & SPI_SR_TXE) {
//			SPI1->DR = accTraBuf[accTraBufIndex];
//		//}
//
//
//		if (accTraBufIndex < 2) {
//			accTraBufIndex++;
//		}
//		for(int i = 0; i < 50000; i++)
//
//		//while (!(SPI1->SR & SPI_SR_TXE))
//
//		if ( accTraBufIndex >= 2) {
//
//
//		//if ( (SPI1->SR & SPI_SR_RXNE) ) {
//			// read from SPI reg (SPI clears bit own its own in MCU)
//			accRcvBuf[0] = SPI1->DR;
//			//accRcvBufIndex++;
//		//}
//
//		}
//
//
//	}


	//SPI1->SR |= SPI_SR_TXE;

}


uint8_t y_acc_low;
uint8_t y_acc_high;
int16_t y_acc;
void SPI1_IRQHandler (void) {

	if (SPI1->SR & SPI_SR_TXE) {

		if (accInitBufIndex < INIT_BUFSIZE) {
			SPI1->DR = accInitBuf[accInitBufIndex];
			accInitBufIndex++;
		} else {


			SPI1->DR = accTraBuf[accTraBufIndex];
			if (accTraBufIndex < (TRANSMIT_BUFSIZE - 1)) {
				accTraBufIndex++;
			} else {
				accTraBufIndex = 0;
			}

		}
	}

	if ( SPI1->SR & SPI_SR_RXNE ) {

		if (accTraBufIndex < TRANSMIT_BUFSIZE - 1) {
			y_acc_low = SPI1->DR;
		} else {
			y_acc_high = SPI1->DR;
			y_acc = ((y_acc_high << 8) | y_acc_low);
		}
		//accRcvBufIndex++;
	}


}

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



