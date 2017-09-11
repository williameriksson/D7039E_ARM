/*
 * initSPI.c
 *
 *  Created on: 1 sep. 2017
 *      Author: rjons
 */
#include "SPI.h"



void InitSPI () {


	__disable_irq();

	//Enables clock for GPIOC and SPI2 interface
	GpioEnable(GPIOB);
	GpioEnable(GPIOA);
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

	//LED enable for tests
//	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	GpioSetOutput(GPIOA, 5);
	//Set AF05 on GPIOB pin 12-15
	GpioSetAF(GPIOB, 12, 5);
	GpioSetAF(GPIOB, 13, 5);
	GpioSetAF(GPIOB, 14, 5);
	GpioSetAF(GPIOB, 15, 5);

	//Sets Output speed to high for PB12-15
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;

	//SPI settings. RX_ONLY enabled
	SPI2->CR1 |= SPI_CR1_RXONLY;

	//Enables recieve not empty interrupt
	SPI2->CR2 |= SPI_CR2_RXNEIE;

	//Enables SPI, This MUST happen after SPI configuration
	SPI2->CR1 |= SPI_CR1_SPE;


	NVIC_EnableIRQ(SPI2_IRQn);
	NVIC_SetPriority(SPI2_IRQn, 36);
	__enable_irq();

}



