/*
 * initSPI.c
 *
 *  Created on: 1 sep. 2017
 *      Author: rjons
 */
#include "initSPI.h"

uint16_t rx;

void initSPI () {

	//Enables clock for GPIOC and SPI2 interface
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB2ENR |= RCC_APB1ENR_SPI2EN;

	//Sets GPIO mode on PB12-15
	GPIOC->MODER |= GPIO_MODER_MODER12_1;
	GPIOC->MODER |= GPIO_MODER_MODER13_1;
	GPIOC->MODER |= GPIO_MODER_MODER14_1;
	GPIOC->MODER |= GPIO_MODER_MODER15_1;

	//Sets PB12-13 to AF6 and PB14-15 to AF5
	GPIOC->AFR[1] |= (GPIO_AF5_SPI2 << 24);
	GPIOC->AFR[1] |= (GPIO_AF5_SPI2 << 28);
	GPIOC->AFR[1] |= (GPIO_AF6_SPI2 << 20);
	GPIOC->AFR[1] |= (GPIO_AF6_SPI2 << 16);

	//SPI settings. TI-Mode, 16-bit Dataframe
	SPI2->CR1 |= SPI_CR1_DFF;
	SPI2->CR2 |= SPI_CR2_FRF;

	//Enables transmit empty and recieve not empty
	//SPI2->CR2 |= SPI_CR2_TXEIE;
	SPI2->CR2 |= SPI_CR2_RXNEIE;




	//Enables SPI, This MUST happen after SPI configuration
	SPI2->CR1 |= SPI_CR1_SPE;

	NVIC_EnableIRQ(SPI2_IRQn);
	NVIC_SetPriority(SPI2_IRQn, 71);

}

void SPI2_IRQHandler (void) {

	if (SPI2->SR & SPI_SR_RXNE) {
		rx = SPI2->DR;
	}



}

