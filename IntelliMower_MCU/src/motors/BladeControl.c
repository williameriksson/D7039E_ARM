/*
 * BladeControl.c
 *
 *  Created on: 7 sep. 2017
 *      Author: A1201-admin
 */
#include "BladeControl.h"

void InitBladeControl () {

	GpioEnable(GPIOA);
	GpioSetOutput(GPIOA, 8);

}

void StartBlades () {

	GpioSetPinHigh(GPIOA, 8);

}

void StopBlades () {

	GpioSetPinLow(GPIOA, 8);

}

