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

void InitI2C();
int I2CRead();
int I2CWrite();



#endif /* COM_I2C_H_ */
