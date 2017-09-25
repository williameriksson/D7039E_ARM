/*
 * I2C.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Sam
 */

#include "I2C.h"

uint8_t testDataLOW;
uint8_t testDataHIGH;
int16_t testData;
void InitI2C() {
	// disable interrupts


	__disable_irq();

	//Enables clock for GPIOB and I2C interface
	GpioEnable(GPIOB);

	RCC->APB1ENR|= RCC_APB1ENR_I2C1EN ; // enable APB1 peripheral clock for I2C1


	//SCL on PB6 and SDA on PB7

//	GPIOB->MODER	|= GPIO_MODER_MODER6; // set pin to alternate function
//	GPIOB->MODER	|= GPIO_MODER_MODER7; // set pin to alternate function
//	GPIOB-> AFR[0] |= GPIO_AF4_I2C1 << 6; // TODO: IS SHIFTING CORRECT? Set PB6 to AF I2C1_SCL
//	GPIOB-> AFR[0] |= GPIO_AF4_I2C1 << 7; // TODO: IS SHIFTING CORRECT? Set PB7 to AF I2C1_SDA
//
	GpioSetAF(GPIOB, 8, 4);  // Set PB6 to AF I2C1_SCL
	GpioSetAF(GPIOB, 9, 4);  // Set PB7 to AF I2C1_SDA

	//GpioSetOutSpeed(GPIOB, 4, GPIO_)
	GpioSetOutSpeed(GPIOB, 8, HIGH);
	GpioSetOutSpeed(GPIOB, 9, HIGH);

	// Below 4 lines needed?
	 GPIOB->OTYPER	|= GPIO_OTYPER_OT_8; // set output to open drain --> the line has to be only pulled low, not driven high
	 GPIOB->OTYPER	|= GPIO_OTYPER_OT_9; // set output to open drain --> the line has to be only pulled low, not driven high

	GPIOB->PUPDR	|= GPIO_PUPDR_PUPDR8_0; // enable pull up resistors
	GPIOB->PUPDR	|= GPIO_PUPDR_PUPDR9_0; // enable pull up resistors

	I2C1->CR2 |= 0b110010; /* 0b110010 == 50Mhz, The FREQ bits must be configured with the APB clock frequency value (I2C peripheral
									connected to APB). The FREQ field is used by the peripheral to generate data setup and
									hold times compliant with the I2C specifications. The minimum allowed frequency is 2 MHz,
									the maximum frequency is limited by the maximum APB1 frequency and cannot exceed
									50 MHz (peripheral intrinsic maximum limit).*/
	//I2C1->CR2 &= ~(I2C_CR1_SMBUS); // I2C mode
	//I2C1->OAR2 = 0x00; 			// Address not relevant in master mode
	I2C1->CR2 |= I2C_CR2_ITERREN;
	I2C1->CR2 |= I2C_CR2_ITEVTEN; // Event interrupt enable
									/* This interrupt is generated when:
										– SB = 1 (Master)
										– ADDR = 1 (Master/Slave)
										– ADD10= 1 (Master)
										– STOPF = 1 (Slave)
										– BTF = 1 with no TxE or RxNE event
										– TxE event to 1 if ITBUFEN = 1
										– RxNE event to 1 if ITBUFEN = 1 */


	I2C1->CCR |= 0x28;
	I2C1->CR1 |= I2C_CR1_PE;    // Enable I2C1;
	I2C1->CR1 |= I2C_CR1_ACK;	// Acknowledge enable, accelerometer/gyro requires ACK mode

	//NVIC_EnableIRQ(I2C1_EV_IRQn);
	//NVIC_SetPriority(I2C1_EV_IRQn, 36);

	__enable_irq();

	//I2C1->CR1 |= I2C_CR1_START;


	I2cWriteByte(SLAVEADDR, CTRL9_XL_ADDR, 0x38);
	I2cWriteByte(SLAVEADDR, CTRL1_XL_ADDR, 0x60);
	I2cWriteByte(SLAVEADDR, INT1_CTRL_ADDR, 0x01);



	while(1) {

		I2cReadByte(SLAVEADDR, OUTX_L_XL, &testDataLOW);
		I2cReadByte(SLAVEADDR, OUTX_H_XL, &testDataHIGH);
		testData = (testDataHIGH << 8) | testDataLOW;


	}




}

#define nrOfWriteMsgs 6
uint8_t writeMsgQ[nrOfWriteMsgs] = {CTRL9_XL_ADDR, 0x38, CTRL1_XL_ADDR, 0x60, INT1_CTRL_ADDR, 0x01};

#define nrOfReadMsgs 2
uint8_t readMsgQ[nrOfWriteMsgs] = {STATUS_REG_ADDR, OUTX_L_XL};

I2C_STATE CURRENT_I2C_STATE = STOP;
I2C_MASTER_ROLE CURRENT_I2C_ROLE = TRANSMITTER;

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

//	if (CURRENT_I2C_STATE == STOP && (SR1 & I2C_SR1_SB)) {
//		CURRENT_I2C_STATE = START;
//	}


}

void I2cWriteByte(uint8_t slave_addr, uint8_t reg_addr, uint8_t data) {
	uint8_t slave_addr_w = slave_addr << 1;
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = slave_addr_w;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	int SR2 = I2C1->SR2;
	I2C1->DR = reg_addr;
	while(!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR = data;
	while(!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->CR1 |= I2C_CR1_STOP;
}

void I2cReadByte(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data) {
	uint8_t slave_addr_r = (slave_addr << 1 | 1);
	uint8_t slave_addr_w = slave_addr << 1;

	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = slave_addr_w;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	int SR2 = I2C1->SR2;
	I2C1->DR = reg_addr;
	while(!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = slave_addr_r;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	I2C1->CR1 &= ~I2C_CR1_ACK;
	SR2 = I2C1->SR2;
	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	*data = I2C1->DR;
	I2C1->CR1 |= I2C_CR1_STOP;
}



