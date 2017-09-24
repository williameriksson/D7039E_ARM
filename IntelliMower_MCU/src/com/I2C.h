/*
 * I2C.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Sam
 */

#ifndef COM_I2C_H_
#define COM_I2C_H_

#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "utils/GPIO.h"

#define WHO_AM_I 0x0F
#define SLAVEADDR					0b1101010
#define CTRL9_XL_ADDR 				0x18
#define CTRL1_XL_ADDR 				0x10
#define INT1_CTRL_ADDR 				0x0D
#define STATUS_REG_ADDR 			0x1E
#define OUTX_L_XL 					0x28
#define OUTX_H_XL 					0x29


typedef enum {
	START,
	SAK_REG_ADDR,
	REPEATED_START,
	RECEIVING,
	READ_LAST_BYTE,
	READY_TO_WRITE,
	STOP
} I2C_STATE;

typedef enum {
	TRANSMITTER,
	RECEIVER
} I2C_MASTER_ROLE;

void InitI2C();
void I2cReadByte(uint8_t, uint8_t, uint8_t*);
void I2cWriteByte(uint8_t, uint8_t, uint8_t);



#endif /* COM_I2C_H_ */
