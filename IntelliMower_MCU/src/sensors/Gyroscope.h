
#ifndef SENSORS_GYROSCOPE_H_
#define SENSORS_GYROSCOPE_H_

#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "com/I2C.h"

#define NDOF_GYRO_ADDR 0x21
#define NDOF_GYRO_CTRL_REG0 0x0D
#define NDOF_GYRO_CTRL_REG1 0x13
#define NDOF_GYRO_CTRL_REG2 0x14
#define NDOF_GYRO_CTRL_REG3 0x15
#define NDOF_GYRO_OUT_X_MSB 0x01

#define NDOF_GYRO_DATA_LEN 6
#define gyroReadInterval 25 //400 Hz @ 10kHz timer

uint8_t gyroDataReady;
uint8_t gyroDataBuffer[NDOF_GYRO_DATA_LEN];
uint8_t gyroCalibrated;


void InitGyro();
void CalibrateGyro(int);
int ReadGyroData();


typedef struct {
	float x;
	float y;
	float z;
	float offsetX;
	float offsetY;
	float offsetZ;
	float pitch;
	float roll;
	float yaw;
	float yawHeading;
} NDOF_GYRO;

NDOF_GYRO gyro;

#endif /* SENSORS_GYROSCOPE_H_ */
