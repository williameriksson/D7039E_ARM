#include "I2C.h"
#include <math.h>
uint8_t testDataLOW;
uint8_t testDataHIGH;
int16_t testData;

uint8_t dataReady = 0;
void InitI2C() {

	__disable_irq();

	//Enables clock for GPIOB and I2C interface
	GpioEnable(GPIOB);
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // enable APB1 peripheral clock for I2C1


	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN; // enable timer 5 for timeouts
	TIM5->PSC = 0x10000 - 1;
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

	initAccMag();
	magCalibration();
	testI2C();

}

void initAccMag() {

	GpioEnable(GPIOA);
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	EXTI->FTSR |= EXTI_FTSR_TR0;
	EXTI->IMR |= EXTI_IMR_MR0;

	NVIC_SetPriority(EXTI0_IRQn, 15);
	NVIC_EnableIRQ(EXTI0_IRQn);

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG2, 0x40); // Reset all registers to POR values
	for(int i = 0; i < 100000; i++);
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_XYZ_DATA_CFG, 0x00); // +/-2g, 0.244mg/LSB
	for(int i = 0; i < 1000; i++);
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_CTRL_REG1, 0x1F); // Hybrid mode (accelerometer + magnetometer), max OSR
	for(int i = 0; i < 1000; i++);
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_CTRL_REG2, 0x20); // M_OUT_X_MSB register 0x33 follows the OUT_Z_LSB register 0x06
	for(int i = 0; i < 1000; i++);
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG2, 0x02); // High resolution mode
	for(int i = 0; i < 1000; i++);

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG4, 0x01); // Enable DRDY interrupt
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG5, 0x01); // DRDY interrupt on INT1
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, 0b00110101); // ODR = 3.125Hz, Reduced noise, Active mode
}


void EXTI0_IRQHandler (void) {
	if (EXTI->PR & EXTI_PR_PR0) {	// Check interrupt flag for PR0
		dataReady = 1;
	}

	EXTI->PR |= EXTI_PR_PR0; 		// clear interrupt flag PR0 by writing 1
}

#define dataLen 12
uint8_t ndof_data[dataLen];
int16_t ndof_x;
int16_t ndof_y;
int16_t ndof_z;
int16_t ndof_m_x;
int16_t ndof_m_y;
int16_t ndof_m_z;
float uTx;
float uTy;
float heading;


void magCalibration() {
	short Xout_Mag_16_bit_avg, Yout_Mag_16_bit_avg, Zout_Mag_16_bit_avg;
	short Xout_Mag_16_bit_max, Yout_Mag_16_bit_max, Zout_Mag_16_bit_max;
	short Xout_Mag_16_bit_min, Yout_Mag_16_bit_min, Zout_Mag_16_bit_min;
	short Xout_Mag_16_bit, Yout_Mag_16_bit, Zout_Mag_16_bit;

	for (int i = 0; i < 30; i++) {
		while(!dataReady);
		dataReady = 0;
		I2cReadMultipleBytes(NDOF_ACC_MAG_ADDR, NDOF_M_OUT_X_MSB, 6, ndof_data);

		Xout_Mag_16_bit = (short) (ndof_data[0]<<8 | ndof_data[1]);        // Compute 16-bit X-axis magnetic output value
		Yout_Mag_16_bit = (short) (ndof_data[2]<<8 | ndof_data[3]);        // Compute 16-bit Y-axis magnetic output value
		Zout_Mag_16_bit = (short) (ndof_data[4]<<8 | ndof_data[5]);        // Compute 16-bit Z-axis magnetic output value

		if (i == 0) {
			Xout_Mag_16_bit_max = Xout_Mag_16_bit;
			Xout_Mag_16_bit_min = Xout_Mag_16_bit;

			Yout_Mag_16_bit_max = Yout_Mag_16_bit;
			Yout_Mag_16_bit_min = Yout_Mag_16_bit;

			Zout_Mag_16_bit_max = Zout_Mag_16_bit;
			Zout_Mag_16_bit_min = Zout_Mag_16_bit;
		}

		// Check to see if current sample is the maximum or minimum X-axis value
		if (Xout_Mag_16_bit > Xout_Mag_16_bit_max)    {Xout_Mag_16_bit_max = Xout_Mag_16_bit;}
		if (Xout_Mag_16_bit < Xout_Mag_16_bit_min)    {Xout_Mag_16_bit_min = Xout_Mag_16_bit;}

		// Check to see if current sample is the maximum or minimum Y-axis value
		if (Yout_Mag_16_bit > Yout_Mag_16_bit_max)    {Yout_Mag_16_bit_max = Yout_Mag_16_bit;}
		if (Yout_Mag_16_bit < Yout_Mag_16_bit_min)    {Yout_Mag_16_bit_min = Yout_Mag_16_bit;}

		// Check to see if current sample is the maximum or minimum Z-axis value
		if (Zout_Mag_16_bit > Zout_Mag_16_bit_max)    {Zout_Mag_16_bit_max = Zout_Mag_16_bit;}
		if (Zout_Mag_16_bit < Zout_Mag_16_bit_min)    {Zout_Mag_16_bit_min = Zout_Mag_16_bit;}

//		for(int j=0; j < 500000; j++);
	}

	Xout_Mag_16_bit_avg = (Xout_Mag_16_bit_max + Xout_Mag_16_bit_min) / 2;            // X-axis hard-iron offset
	Yout_Mag_16_bit_avg = (Yout_Mag_16_bit_max + Yout_Mag_16_bit_min) / 2;            // Y-axis hard-iron offset
	Zout_Mag_16_bit_avg = (Zout_Mag_16_bit_max + Zout_Mag_16_bit_min) / 2;            // Z-axis hard-iron offset


	// Left-shift by one as magnetometer offset registers are 15-bit only, left justified
	Xout_Mag_16_bit_avg <<= 1;
	Yout_Mag_16_bit_avg <<= 1;
	Zout_Mag_16_bit_avg <<= 1;

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, 0x00);          // Standby mode to allow writing to the offset registers

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_X_LSB, (char) (Xout_Mag_16_bit_avg & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_X_MSB, (char) ((Xout_Mag_16_bit_avg >> 8) & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Y_LSB, (char) (Yout_Mag_16_bit_avg & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Y_MSB, (char) ((Yout_Mag_16_bit_avg >> 8) & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Z_LSB, (char) (Zout_Mag_16_bit_avg & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Z_MSB, (char) ((Zout_Mag_16_bit_avg >> 8) & 0xFF));

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, 0b00110101);          // Active mode again

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
	dataReady = 1;
}


void testI2C() {
	while (1) {
		while(!dataReady);
		dataReady = 0;
		I2cReadMultipleBytes(NDOF_ACC_MAG_ADDR, NDOF_OUT_X_MSB, dataLen, ndof_data);
		ndof_x = ((short) (ndof_data[0]<<8 | ndof_data[1])) >> 2;
		ndof_y = ((short) (ndof_data[2]<<8 | ndof_data[3])) >> 2;
		ndof_z = ((short) (ndof_data[4]<<8 | ndof_data[5])) >> 2;
		ndof_m_x = (short) (ndof_data[6]<<8 | ndof_data[7]);
		ndof_m_y = (short) (ndof_data[8]<<8 | ndof_data[9]);
		ndof_m_z = (short) (ndof_data[10]<<8 | ndof_data[11]);

		uTx = (float) (ndof_m_x) / SENSITIVITY_MAG;
		uTy = (float) (ndof_m_y) / SENSITIVITY_MAG;
		heading = 180 + atan2(uTy, uTx) * 180 / 3.141592;

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



