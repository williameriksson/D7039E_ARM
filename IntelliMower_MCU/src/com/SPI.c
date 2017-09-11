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
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

	//LED enable for tests
//	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	GpioSetOutput(GPIOA, 5);
	//Set AF05 on pin 12-15
	GpioSetAF(GPIOB, 12, 5);
	GpioSetAF(GPIOB, 13, 5);
	GpioSetAF(GPIOB, 14, 5);
	GpioSetAF(GPIOB, 15, 5);
//	GPIOB->MODER |= GPIO_MODER_MODER12_1;
//	GPIOB->MODER |= GPIO_MODER_MODER13_1;
//	GPIOB->MODER |= GPIO_MODER_MODER14_1;
//	GPIOB->MODER |= GPIO_MODER_MODER15_1;

	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;

	//Sets PB12-13 to AF6 and PB14-15 to AF5
//	GPIOB->AFR[1] |= (GPIO_AF5_SPI2 << 24);
//	GPIOB->AFR[1] |= (GPIO_AF5_SPI2 << 28);
//	GPIOB->AFR[1] |= (GPIO_AF5_SPI2 << 20);
//	GPIOB->AFR[1] |= (GPIO_AF5_SPI2 << 16);

	//SPI settings. TI-Mode, 16-bit Dataframe
	//SPI2->CR1 |= SPI_CR1_DFF;
	SPI2->CR1 |= SPI_CR1_RXONLY;
	//SPI2->CR2 |= SPI_CR2_FRF;

	//Enables transmit empty and recieve not empty
	//SPI2->CR2 |= SPI_CR2_TXEIE;
	SPI2->CR2 |= SPI_CR2_RXNEIE;

	//GPIOA->ODR |= GPIO_ODR_ODR_5;




	//Enables SPI, This MUST happen after SPI configuration
	SPI2->CR1 |= SPI_CR1_SPE;


	NVIC_EnableIRQ(SPI2_IRQn);
	NVIC_SetPriority(SPI2_IRQn, 36);
	__enable_irq();

}



