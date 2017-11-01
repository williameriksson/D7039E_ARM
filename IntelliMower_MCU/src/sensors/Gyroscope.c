#include "Gyroscope.h"
#include <math.h>

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
	gyroCalibrated = 0;
	I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG1, 0x40); // Reset device
	for (int i = 0; i < 10000; i++);
	I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG0, 0x03); // +/-250dps range

	//I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG2, 0x0C); // DRDY interrupt enable and routed to INT1
	I2cWriteByte(NDOF_GYRO_ADDR, NDOF_GYRO_CTRL_REG1, 0x06); // 400Hz, Active mode
}

uint32_t nextGyroRead = gyroReadInterval;
uint32_t startTime;
float xAngle, yAngle, zAngle;

void CalibrateGyro(int numberOfSamples) {
	float n = 1.0;
	float avgX = 0.0;
	float avgY = 0.0;
	float avgZ = 0.0;

	while (n <= numberOfSamples) {
		int isOldData = ReadGyroData();
		if (isOldData) {
			continue;
		}
		avgX = avgX + (gyro.x - avgX) / n;
		avgY = avgY + (gyro.y - avgY) / n;
		avgZ = avgZ + (gyro.z - avgZ) / n;
		n++;

	}
	gyro.offsetX = avgX;
	gyro.offsetY = avgY;
	gyro.offsetZ = avgZ;
	gyroCalibrated = 1;

}

int ReadGyroData() {
//	if(!gyroDataReady) {return;}
//	gyroDataReady = 0;
	if (!(TIM5->CNT > nextGyroRead)) {return 1;}
	nextGyroRead = TIM5->CNT + gyroReadInterval;
	I2cReadMultipleBytes(NDOF_GYRO_ADDR, NDOF_GYRO_OUT_X_MSB, NDOF_GYRO_DATA_LEN, gyroDataBuffer);
	float tmpX = (float) ( (int16_t) ((gyroDataBuffer[0]<<8 | gyroDataBuffer[1]))) / 131.072;// * 0.00109083078;
	float tmpY = (float) ( (int16_t) ((gyroDataBuffer[2]<<8 | gyroDataBuffer[3]))) / 131.072;// * 0.00109083078;
	float tmpZ = (float) ( (int16_t) ((gyroDataBuffer[4]<<8 | gyroDataBuffer[5]))) / 131.072;// * 0.00109083078;

	gyro.x = tmpX - gyro.offsetX; 	// 1.282 average of ~150000 samples
	gyro.y = tmpY - gyro.offsetY;	// 0.382 average of ~150000 samples
	gyro.z = tmpZ - gyro.offsetZ;	// 0.19 average of ~150000 samples


	uint32_t endTime = TIM5->CNT;
	float dt = ((endTime - startTime) / 10000.0);
	startTime = endTime;
	if (gyroCalibrated) {
		xAngle = xAngle + gyro.x * dt;
		yAngle = yAngle + gyro.y * dt;
		zAngle = zAngle + gyro.z * dt;
	}
	return 0;

}
