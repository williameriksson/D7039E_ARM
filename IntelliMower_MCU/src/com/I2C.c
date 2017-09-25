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

	I2C1->CR2 |= I2C_CR2_ITBUFEN;

	I2C1->CCR |= 0x28;
	I2C1->CR1 |= I2C_CR1_PE;    // Enable I2C1;
	I2C1->CR1 |= I2C_CR1_ACK;	// Acknowledge enable, accelerometer/gyro requires ACK mode


	I2cWriteByte(SLAVEADDR, CTRL9_XL_ADDR, 0x38);
	I2cWriteByte(SLAVEADDR, CTRL1_XL_ADDR, 0x60);
	I2cWriteByte(SLAVEADDR, INT1_CTRL_ADDR, 0x01);

	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_SetPriority(I2C1_EV_IRQn, 36);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
	NVIC_SetPriority(I2C1_ER_IRQn, 36);


	__enable_irq();



	testI2C();

	//I2C1->CR1 |= I2C_CR1_START;
//
//
//
//	while(1) {
//
//		I2cReadByte(SLAVEADDR, OUTX_L_XL, &testDataLOW);
//		I2cReadByte(SLAVEADDR, OUTX_H_XL, &testDataHIGH);
//		testData = (testDataHIGH << 8) | testDataLOW;
//
//
//	}




}


I2C_STATE CURRENT_I2C_STATE = STOP;
I2C_MASTER_ROLE CURRENT_I2C_ROLE = TRANSMITTER;
int nrErrors = 0;
int nrInterrupts = 0;
void I2C1_ER_IRQHandler(void) {
	nrErrors++;
	nrErrors++;
	CURRENT_I2C_STATE = STOP;
	I2C1->SR1 &= ~(I2C_SR1_BERR 	|
				   I2C_SR1_ARLO 	|
				   I2C_SR1_AF 		|
				   I2C_SR1_OVR 		|
				   I2C_SR1_PECERR 	|
				   I2C_SR1_TIMEOUT 	|
				   I2C_SR1_SMBALERT);

}

#define nrOfInitBytes 6
uint8_t initBytes[nrOfInitBytes] = {CTRL9_XL_ADDR, 0x38, CTRL1_XL_ADDR, 0x60, INT1_CTRL_ADDR, 0x01};

#define nrOfReadBytes 1
uint8_t readBytes[nrOfReadBytes] = {OUTX_H_XL};

int16_t data;
int initCount = 0;
int repeatedStart = 0;
void I2C1_EV_IRQHandler (void) {
	nrInterrupts++;

	if (I2C1->SR1 & I2C_SR1_SB) {
		if (repeatedStart == 1) {
			I2C1->DR = ((SLAVEADDR << 1) | 1);
		} else {
			I2C1->DR = (SLAVEADDR << 1);
		}
	}

	else if (I2C1->SR1 & I2C_SR1_ADDR) {
		if (repeatedStart == 1) {
			I2C1->CR1 &= ~I2C_CR1_ACK;
			int SR2 = I2C1->SR2;
		} else {
			int SR2 = I2C1->SR2;
			I2C1->DR = readBytes[0];
		}

	}

	else if ((I2C1->SR1 & I2C_SR1_TXE) && !(I2C1->SR1 & I2C_SR1_ADDR)) {
		I2C1->CR1 |= I2C_CR1_START;
		repeatedStart = 1;
	}

	else if (I2C1->SR1 & I2C_SR1_RXNE) {
		data = (I2C1->DR << 8);
		repeatedStart = 0;
		I2C1->CR1 |= I2C_CR1_STOP;
	}

}

void testI2C() {
	while (1) {
		for(int i = 0; i < 10000000; i++);
		I2C1->CR1 |= I2C_CR1_ACK;
		I2C1->CR1 |= I2C_CR1_START;

	}
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



