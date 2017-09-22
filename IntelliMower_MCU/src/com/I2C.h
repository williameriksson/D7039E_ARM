/*
 * I2C.h
 *
 *  Created on: 18 sep. 2017
 *      Author: Sam
 */

#ifndef COM_I2C_H_
#define COM_I2C_H_

#define ACC_GYRO_OUTX_H_G  			0x23
#define ACC_GYRO_OUTX_L_G  			0X22
#define SLAVEADDRREAD				0b11010101
#define SLAVEADDRWRITE				0b11010100

void InitI2C();
int I2CRead();
int I2CWrite();



#endif /* COM_I2C_H_ */
