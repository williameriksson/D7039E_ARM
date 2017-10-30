#include "Gyroscope.h"

void EXTI9_5_IRQHandler (void) {
	if (EXTI->PR & EXTI_PR_PR8) {	// Check interrupt flag for PR8
		gyroDataReady = 1;
	}

	EXTI->PR |= EXTI_PR_PR8; 		// clear interrupt flag PR8 by writing 1
}

void InitGyro() {
//	GpioEnable(GPIOA);
//	SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PA;
//	EXTI->FTSR |= EXTI_FTSR_TR8;
//	EXTI->IMR |= EXTI_IMR_MR8;

//	NVIC_SetPriority(EXTI9_5_IRQn, 16);
//	NVIC_EnableIRQ(EXTI9_5_IRQn);

	I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG1, 0x40); // Reset device
	for (int i = 0; i < 10000; i++);
	I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG0, 0x07); // +/-250dps range, High pass filter enable

	//I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG2, 0x0C); // DRDY interrupt enable and routed to INT1
	I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG1, 0x06); // 400Hz, Active mode
}

uint32_t nextGyroRead = gyroReadInterval;
void ReadGyroData() {
//	if(!gyroDataReady) {return;}
//	gyroDataReady = 0;
	if (!(TIM5->CNT > nextGyroRead)) {return;}
	nextGyroRead = TIM5->CNT + gyroReadInterval;
	I2cReadMultipleBytes(NDOF_GYRO_ADDR, NDOF_GYRO_OUT_X_MSB, NDOF_GYRO_DATA_LEN, gyroDataBuffer);
	gyro.x = (float) ( (int16_t) ((gyroDataBuffer[0]<<8 | gyroDataBuffer[1]))) / 131.072;// * 0.00109083078;
	gyro.y = (float) ( (int16_t) ((gyroDataBuffer[2]<<8 | gyroDataBuffer[3]))) / 131.072;// * 0.00109083078;
	gyro.z = (float) ( (int16_t) ((gyroDataBuffer[4]<<8 | gyroDataBuffer[5]))) / 131.072;// * 0.00109083078;


	//	if (accCalibrated) {
	//		endTime = TIM5->CNT;
	//		dt = ((endTime - startTime) / 10000.0);
	//		startTime = endTime;
	//		xVel = xVel + ndof.accX * dt;
	//		xPos = xPos + xVel * dt;
	//		yVel = yVel + ndof.accY * dt;
	//		yPos = yPos + yVel * dt;
	//	}
}
