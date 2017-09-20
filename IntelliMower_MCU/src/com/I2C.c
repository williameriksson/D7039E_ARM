/*
 * I2C.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Sam
 */

#include "I2C.h"


void InitI2C() {
	// disable interrupts
	__disable_irq();

	//Enables clock for GPIOB and I2C interface
	GpioEnable(GPIOB);

	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN ; // enable APB1 peripheral clock for I2C1

	//SCL on PB6 and SDA on PB7
	/*
	GPIOB->MODER	|= GPIO_MODER_MODER6; // set pin to alternate function
	GPIOB->MODER	|= GPIO_MODER_MODER7; // set pin to alternate function
	GPIOB-> AFR[0] |= GPIO_AF4_I2C1 << 6; // TODO: IS SHIFTING CORRECT? Set PB6 to AF I2C1_SCL
	GPIOB-> AFR[0] |= GPIO_AF4_I2C1 << 7; // TODO: IS SHIFTING CORRECT? Set PB7 to AF I2C1_SDA
	*/
	GpioSetAF(GPIOB, 6, 4);  // Set PB6 to AF I2C1_SCL
	GpioSetAF(GPIOB, 7, 4);  // Set PB7 to AF I2C1_SDA

	//GpioSetOutSpeed(GPIOB, 4, GPIO_)

	GPIOB->OSPEEDR	|= GPIO_OSPEEDER_OSPEEDR6; //set GPIO speed TODO: replace with GpioSetOutSpeed...
	GPIOB->OSPEEDR	|= GPIO_OSPEEDER_OSPEEDR7; //set GPIO speed TODO: replace with GpioSetOutSpeed...

	// Below 4 lines needed?
	GPIOB->OTYPER	|= GPIO_OTYPER_OT_6; // set output to open drain --> the line has to be only pulled low, not driven high
	GPIOB->OTYPER	|= GPIO_OTYPER_OT_7; // set output to open drain --> the line has to be only pulled low, not driven high

	GPIOB->PUPDR	|= GPIO_PUPDR_PUPDR6_0; // enable pull up resistors
	GPIOB->PUPDR	|= GPIO_PUPDR_PUPDR7_0; // enable pull up resistors






	I2C1->CR2 |= 0b110010; /* 0b110010 == 50Mhz, The FREQ bits must be configured with the APB clock frequency value (I2C peripheral
									connected to APB). The FREQ field is used by the peripheral to generate data setup and
									hold times compliant with the I2C specifications. The minimum allowed frequency is 2 MHz,
									the maximum frequency is limited by the maximum APB1 frequency and cannot exceed
									50 MHz (peripheral intrinsic maximum limit).*/
	//I2C1->CR2 &= ~(I2C_CR1_SMBUS); // I2C mode
	//I2C1->OAR2 = 0x00; 			// Address not relevant in master mode
	I2C1->CR2 |= I2C_CR2_ITEVTEN; // Event interrupt enable
									/* This interrupt is generated when:
										– SB = 1 (Master)
										– ADDR = 1 (Master/Slave)
										– ADD10= 1 (Master)
										– STOPF = 1 (Slave)
										– BTF = 1 with no TxE or RxNE event
										– TxE event to 1 if ITBUFEN = 1
										– RxNE event to 1if ITBUFEN = 1 */


	I2C1->CR1 |= I2C_CR1_PE;    // Enable I2C1;




	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_SetPriority(I2C1_EV_IRQn, 36);

	__enable_irq();
}

