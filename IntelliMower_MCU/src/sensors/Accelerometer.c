#include "Accelerometer.h"
#include <math.h>


void EXTI0_IRQHandler (void) {
	if (EXTI->PR & EXTI_PR_PR0) {	// Check interrupt flag for PR0
		accDataReady = 1;
	}

	EXTI->PR |= EXTI_PR_PR0; 		// clear interrupt flag PR0 by writing 1
}

void InitAccMag() {

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
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, NDOF_ACTIVE_VAL); // ODR = 3.125Hz, Reduced noise, Active mode
}

void AccCalibration() {
	int8_t xAccelOffset, yAccelOffset, zAccelOffset;
	int16_t xOutAccel14Bit, yOutAccel14Bit, zOutAccel14Bit;
	int32_t tmpXOutAccel, tmpYOutAccel, tmpZOutAccel;

	int nrOfSamples = 5000;
	for (int i = 0; i < nrOfSamples; i++) {

		ReadAccMagData();

		tmpXOutAccel += ( (int16_t) (ndofDataBuffer[0]<<8 | ndofDataBuffer[1])) >> 2;  // Compute 14-bit X-axis acc output value
		tmpYOutAccel += ( (int16_t) (ndofDataBuffer[2]<<8 | ndofDataBuffer[3])) >> 2;  // Compute 14-bit Y-axis acc output value
		tmpZOutAccel += ( (int16_t) (ndofDataBuffer[4]<<8 | ndofDataBuffer[5])) >> 2;  // Compute 14-bit Z-axis acc output value
	}

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, 0x00); // Standby mode

	tmpXOutAccel /= nrOfSamples;
	tmpYOutAccel /= nrOfSamples;
	tmpZOutAccel /= nrOfSamples;

	xOutAccel14Bit = tmpXOutAccel;
	yOutAccel14Bit = tmpYOutAccel;
	zOutAccel14Bit = tmpZOutAccel;




//	xAccelOffset = -14; 						// Compute X-axis offset correction value
//	yAccelOffset = 16; 						// Compute X-axis offset correction value
//	zAccelOffset = -14;	// Compute X-axis offset correction value

//	xAccelOffset = (xOutAccel14Bit & 0x00FF) * (-1); 						// Compute X-axis offset correction value
//	yAccelOffset = (yOutAccel14Bit & 0x00FF) * (-1); 						// Compute X-axis offset correction value
//	zAccelOffset = ((zOutAccel14Bit - SENSITIVITY_2G) & 0x00FF) * (-1);	// Compute X-axis offset correction value

	xAccelOffset = xOutAccel14Bit / 8 * (-1); 						// Compute X-axis offset correction value
	yAccelOffset = yOutAccel14Bit / 8 * (-1); 						// Compute X-axis offset correction value
	zAccelOffset = (zOutAccel14Bit - SENSITIVITY_2G) / 8 * (-1);	// Compute X-axis offset correction value




	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_OFF_X, xAccelOffset);
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_OFF_Y, yAccelOffset);
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_OFF_Z, zAccelOffset);

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, NDOF_ACTIVE_VAL); // Active mode again

}

void MagCalibration() {
	short xOutMag16BitAvg, yOutMag16BitAvg, zOutMag16BitAvg;
	short xOutMag16BitMax, yOutMag16BitMax, zOutMag16BitMax;
	short xOutMag16BitMin, yOutMag16BitMin, zOutMag16BitMin;
	short xOutMag16Bit, yOutMag16Bit, zOutMag16Bit;

	for (int i = 0; i < 5000; i++) {
		while(!accDataReady);
		accDataReady = 0;
		I2cReadMultipleBytes(NDOF_ACC_MAG_ADDR, NDOF_M_OUT_X_MSB, 6, ndofDataBuffer);

		xOutMag16Bit = (int16_t) (ndofDataBuffer[0]<<8 | ndofDataBuffer[1]);   // Compute 16-bit X-axis magnetic output value
		yOutMag16Bit = (int16_t) (ndofDataBuffer[2]<<8 | ndofDataBuffer[3]);   // Compute 16-bit Y-axis magnetic output value
		zOutMag16Bit = (int16_t) (ndofDataBuffer[4]<<8 | ndofDataBuffer[5]);   // Compute 16-bit Z-axis magnetic output value

		if (i == 0) {
			xOutMag16BitMax = xOutMag16Bit;
			xOutMag16BitMin = xOutMag16Bit;

			yOutMag16BitMax = yOutMag16Bit;
			yOutMag16BitMin = yOutMag16Bit;

			zOutMag16BitMax = zOutMag16Bit;
			zOutMag16BitMin = zOutMag16Bit;
		}

		// Check to see if current sample is the maximum or minimum X-axis value
		if (xOutMag16Bit > xOutMag16BitMax)    {xOutMag16BitMax = xOutMag16Bit;}
		if (xOutMag16Bit < xOutMag16BitMin)    {xOutMag16BitMin = xOutMag16Bit;}

		// Check to see if current sample is the maximum or minimum Y-axis value
		if (yOutMag16Bit > yOutMag16BitMax)    {yOutMag16BitMax = yOutMag16Bit;}
		if (yOutMag16Bit < yOutMag16BitMin)    {yOutMag16BitMin = yOutMag16Bit;}

		// Check to see if current sample is the maximum or minimum Z-axis value
		if (zOutMag16Bit > zOutMag16BitMax)    {zOutMag16BitMax = zOutMag16Bit;}
		if (zOutMag16Bit < zOutMag16BitMin)    {zOutMag16BitMin = zOutMag16Bit;}

	}

	xOutMag16BitAvg = (xOutMag16BitMax + xOutMag16BitMin) / 2;            // X-axis hard-iron offset
	yOutMag16BitAvg = (yOutMag16BitMax + yOutMag16BitMin) / 2;            // Y-axis hard-iron offset
	zOutMag16BitAvg = (zOutMag16BitMax + zOutMag16BitMin) / 2;            // Z-axis hard-iron offset


	// Left-shift by one as magnetometer offset registers are 15-bit only, left justified
	xOutMag16BitAvg <<= 1;
	yOutMag16BitAvg <<= 1;
	zOutMag16BitAvg <<= 1;

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, 0x00);          // Standby mode to allow writing to the offset registers

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_X_LSB, (char) (xOutMag16BitAvg & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_X_MSB, (char) ((xOutMag16BitAvg >> 8) & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Y_LSB, (char) (yOutMag16BitAvg & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Y_MSB, (char) ((yOutMag16BitAvg >> 8) & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Z_LSB, (char) (zOutMag16BitAvg & 0xFF));
	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_M_OFF_Z_MSB, (char) ((zOutMag16BitAvg >> 8) & 0xFF));

	I2cWriteByte(NDOF_ACC_MAG_ADDR, NDOF_CTRL_REG1, NDOF_ACTIVE_VAL);          // Active mode again

}

void ReadAccMagData() {

	while(!accDataReady);
	accDataReady = 0;
	I2cReadMultipleBytes(NDOF_ACC_MAG_ADDR, NDOF_OUT_X_MSB, NDOF_DATA_LEN, ndofDataBuffer);
	ndof.accX = (float) ( (int16_t) ((ndofDataBuffer[0]<<8 | ndofDataBuffer[1])) >> 2) / SENSITIVITY_2G;
	ndof.accY = (float) ( (int16_t) ((ndofDataBuffer[2]<<8 | ndofDataBuffer[3])) >> 2) / SENSITIVITY_2G;
	ndof.accZ = (float) ( (int16_t) ((ndofDataBuffer[4]<<8 | ndofDataBuffer[5])) >> 2) / SENSITIVITY_2G;
	ndof.magX = (float) ( (int16_t) (ndofDataBuffer[6]<<8 | ndofDataBuffer[7])) / SENSITIVITY_MAG;
	ndof.magY = (float) ( (int16_t) (ndofDataBuffer[8]<<8 | ndofDataBuffer[9])) / SENSITIVITY_MAG;
	ndof.magZ = (float) ( (int16_t) (ndofDataBuffer[10]<<8 | ndofDataBuffer[11])) / SENSITIVITY_MAG;

	ndof.heading = 180 + atan2(ndof.magY, ndof.magX) * 180 / 3.141592;

}
