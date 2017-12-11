/*
 * Common.h
 *
 *  Created on: 7 sep. 2017
 *      Author: A1201-admin
 */


#ifndef FRAMEWORK_COMMON_H_
#define FRAMEWORK_COMMON_H_

#include "com/SPI.h"
#include "framework/ControlLoop.h"
#include "framework/CmdSystem.h"
#include "motors/Steering.h"
#include "sensors/UltraSonic.h"
#include "utils/containers/CircularBuffer.h"
#include "utils/containers/Queue.h"
#include "utils/containers/Stack.h"
#include "com/I2C.h"
#include "com/UART.h"
#include "SafetyControl.h"
#include "sensors/Accelerometer.h"
#include "sensors/Gyroscope.h"
#include "sensors/Encoder.h"
#include "utils/Geometry.h"
#include "demos/demos.h"


void 	CommonInit(); 				// master init
int 	CommonIsInitialized();		// init check
void 	CommonFrame(); 				// "main loop"
void 	CommonQuit();				// quit the program



#endif /* FRAMEWORK_COMMON_H_ */
