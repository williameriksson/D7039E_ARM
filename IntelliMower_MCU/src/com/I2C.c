/*
 * I2C.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Sam
 */

#include "I2C.h"
#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "utils/GPIO.h"


void InitI2C() {
	// disable interrupts
	__disable_irq();

	//Enables clock for GPIOB and I2C interface
	GpioEnable(GPIOB);
	//RCC->APB1ENR |= RCC_APB1ENR_I2C;

	// Initialize the I2C1 module with 100Kbps (or fast mode 400Kbps) speed on PORTB
	//I2C1_Init_Advanced(100000, &_GPIO_MODULE_I2C1_PB67);

	//Set AF on GPIOB pins
	//GpioSetAF(GPIOB, 12, 5);

	//Sets Output speed to high for PB8-9
	//GpioSetOutSpeed(GPIOB, 8, LOW);
	//GpioSetOutSpeed(GPIOB, 9, LOW);


	//SPI settings. RX_ONLY enabled
	SPI2->CR1 |= SPI_CR1_RXONLY;

	//Enables recieve not empty interrupt
	SPI2->CR2 |= SPI_CR2_RXNEIE;

	//Enables SPI, This MUST happen after SPI configuration
	SPI2->CR1 |= SPI_CR1_SPE;

	// configure interrupt
	//NVIC_EnableIRQ(SPI2_IRQn);
	//NVIC_SetPriority(SPI2_IRQn, 36);

	// reenable interrupts
	__enable_irq();
}

