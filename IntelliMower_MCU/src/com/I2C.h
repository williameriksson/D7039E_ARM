#ifndef COM_I2C_H_
#define COM_I2C_H_

#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"
#include "utils/GPIO.h"

void InitI2C();
void I2cReadByte(uint8_t, uint8_t, uint8_t*);
void I2cReadMultipleBytes(uint8_t, uint8_t, int, uint8_t[]);
void I2cWriteByte(uint8_t, uint8_t, uint8_t);
void I2CBusReset();
uint8_t hasTimedOut(uint32_t startTime, uint32_t timeOut);

#endif /* COM_I2C_H_ */
