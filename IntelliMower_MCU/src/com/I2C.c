#include "I2C.h"
#include "sensors/Accelerometer.h" // For the dataReady reset, might want to change this later
#include "sensors/Gyroscope.h"

void InitI2C() {

	__disable_irq();

	//Enables clock for GPIOB and I2C interface
	GpioEnable(GPIOB);
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // enable APB1 peripheral clock for I2C1


	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; // enable timer 5 for timeouts
	TIM5->PSC = 10000 - 1;
	TIM5->ARR = 0xFFFFFFFF;
	TIM5->CR1 |= TIM_CR1_CEN;
	TIM5->EGR |= TIM_EGR_UG;

	GpioSetAF(GPIOB, 8, 4);  // Set PB8 to AF I2C1_SCL
	GpioSetAF(GPIOB, 9, 4);  // Set PB9 to AF I2C1_SDA

	GpioSetOutSpeed(GPIOB, 8, HIGH);
	GpioSetOutSpeed(GPIOB, 9, HIGH);

	GPIOB->OTYPER	|= GPIO_OTYPER_OT_8; // set output to open drain --> the line has to be only pulled low, not driven high
	GPIOB->OTYPER	|= GPIO_OTYPER_OT_9; // set output to open drain --> the line has to be only pulled low, not driven high

	GPIOB->PUPDR	|= GPIO_PUPDR_PUPDR8_0; // enable pull up resistors
	GPIOB->PUPDR	|= GPIO_PUPDR_PUPDR9_0; // enable pull up resistors

	I2C1->CR2 |= 0b110010; /* 0b110010 == 50Mhz, The FREQ bits must be configured with the APB clock frequency value (I2C peripheral
									connected to APB). The FREQ field is used by the peripheral to generate data setup and
									hold times compliant with the I2C specifications. The minimum allowed frequency is 2 MHz,
									the maximum frequency is limited by the maximum APB1 frequency and cannot exceed
									50 MHz (peripheral intrinsic maximum limit).*/

	//I2C1->CR2 |= I2C_CR2_ITERREN;
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

	I2C1->CCR |= I2C_CCR_FS;
	I2C1->TRISE |= 0x09;
	I2C1->CCR |= 0x28;
	I2C1->CR1 |= I2C_CR1_PE;    // Enable I2C1;
	I2C1->CR1 |= I2C_CR1_ACK;	// Acknowledge enable, accelerometer/gyro requires ACK mode

	__enable_irq();

}


uint8_t hasTimedOut(uint32_t startTime, uint32_t timeOut) {
	if (((TIM5->CNT) - startTime) > timeOut) {
		return 1;
	}
	return 0;
}

void I2CBusReset() {
	I2C1->CR1 |= I2C_CR1_STOP;
	for(int i = 0; i < 5000; i++);
	I2C1->CR1 &= ~I2C_CR1_PE;
	for(int i = 0; i < 5000; i++);
	I2C1->CR1 |= I2C_CR1_PE;
	accDataReady = 1;
	gyroDataReady = 1;
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
	uint32_t startTime = TIM5->CNT;
	uint32_t timeOut = 1000;

	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB) && !hasTimedOut(startTime, timeOut));
	I2C1->DR = slave_addr_w;
	while(!(I2C1->SR1 & I2C_SR1_ADDR) && !hasTimedOut(startTime, timeOut));
	int SR2 = I2C1->SR2;
	I2C1->DR = reg_addr;
	while(!(I2C1->SR1 & I2C_SR1_TXE) && !hasTimedOut(startTime, timeOut));
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB) && !hasTimedOut(startTime, timeOut));
	I2C1->DR = slave_addr_r;
	while(!(I2C1->SR1 & I2C_SR1_ADDR) && !hasTimedOut(startTime, timeOut));
	I2C1->CR1 &= ~I2C_CR1_ACK;
	SR2 = I2C1->SR2;
	while(!(I2C1->SR1 & I2C_SR1_RXNE) && !hasTimedOut(startTime, timeOut));
	*data = I2C1->DR;
	I2C1->CR1 |= I2C_CR1_STOP;

	if (hasTimedOut(startTime, timeOut)) {
		I2CBusReset();
	}

}


void I2cReadMultipleBytes(uint8_t slave_addr, uint8_t reg_addr, int nrOfBytes, uint8_t data[]) {

	uint8_t slave_addr_r = (slave_addr << 1 | 1);
	uint8_t slave_addr_w = slave_addr << 1;

	uint32_t startTime = TIM5->CNT;
	uint32_t timeOut = 1000;

	I2C1->CR1 |= I2C_CR1_ACK;
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB) && !hasTimedOut(startTime, timeOut));
	I2C1->DR = slave_addr_w;
	while(!(I2C1->SR1 & I2C_SR1_ADDR) && !hasTimedOut(startTime, timeOut));
	int SR2 = I2C1->SR2;
	I2C1->DR = reg_addr;
	while(!(I2C1->SR1 & I2C_SR1_TXE) && !hasTimedOut(startTime, timeOut));
	I2C1->CR1 |= I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB) && !hasTimedOut(startTime, timeOut));
	I2C1->DR = slave_addr_r;
	while(!(I2C1->SR1 & I2C_SR1_ADDR) && !hasTimedOut(startTime, timeOut));
	SR2 = I2C1->SR2;
	for (int i = 0; i < nrOfBytes - 1; i++) {
		while(!(I2C1->SR1 & I2C_SR1_RXNE) && !hasTimedOut(startTime, timeOut));
		startTime = TIM5->CNT;
		data[i] = I2C1->DR;
	}
	I2C1->CR1 &= ~I2C_CR1_ACK;
	while(!(I2C1->SR1 & I2C_SR1_RXNE) && !hasTimedOut(startTime, timeOut));
	data[nrOfBytes - 1] = I2C1->DR;
	I2C1->CR1 |= I2C_CR1_STOP;

	if (hasTimedOut(startTime, timeOut)) {
		I2CBusReset();
	}

}



