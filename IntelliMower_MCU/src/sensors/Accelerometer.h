#ifndef SENSORS_ACCELEROMETER_H_
#define SENSORS_ACCELEROMETER_H_

#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "com/I2C.h"
#include "utils/GPIO.h"

#define NDOF_ACC_MAG_ADDR 0x1F
#define NDOF_CTRL_REG1 0x2A
#define NDOF_CTRL_REG2 0x2B
#define NDOF_CTRL_REG3 0x2C
#define NDOF_CTRL_REG4 0x2D
#define NDOF_CTRL_REG5 0x2E
#define NDOF_XYZ_DATA_CFG 0x0E
#define NDOF_OUT_X_MSB 0x01
#define NDOF_OFF_X 0x2F
#define NDOF_OFF_Y 0x30
#define NDOF_OFF_Z 0x31

#define NDOF_M_OUT_X_MSB 0x33
#define NDOF_M_CTRL_REG1 0x5B
#define NDOF_M_CTRL_REG2 0x5C
#define NDOF_M_OFF_X_MSB 0x3F
#define NDOF_M_OFF_X_LSB 0x40
#define NDOF_M_OFF_Y_MSB 0x41
#define NDOF_M_OFF_Y_LSB 0x42
#define NDOF_M_OFF_Z_MSB 0x43
#define NDOF_M_OFF_Z_LSB 0x44

#define NDOF_ACTIVE_VAL 0b00000101

#define SENSITIVITY_2G 4096
#define SENSITIVITY_4G 2048
#define SENSITIVITY_8G 1024
#define SENSITIVITY_MAG 10

#define accReadInterval 25 //400 Hz @ 10kHz timer


#define NDOF_DATA_LEN 12

int ReadAccMagData();
void CalibrateAcc(int);
void CalibrateMag(int);
void InitAccMag();

uint8_t accDataReady;
uint8_t accCalibrated;
uint8_t magCalibrated;
uint8_t ndofDataBuffer[NDOF_DATA_LEN];

typedef struct {
	float accX;
	float accY;
	float accZ;
	float magX;
	float magY;
	float magZ;
	float heading;
} NDOF;

NDOF ndof;


#endif /* SENSORS_ACCELEROMETER_H_ */
