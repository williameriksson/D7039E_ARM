/*
 * BladeControl.h
 *
 *  Created on: 21 sep. 2017
 *      Author: rjons
 */

#ifndef MOTORS_BLADECONTROL_H_
#define MOTORS_BLADECONTROL_H_

#include "stm32f411xe.h"
#include "utils/Timer.h"
#include "utils/GPIO.h"

void InitBladeControl();
void StartBlades();
void StopBlades();



#endif /* MOTORS_BLADECONTROL_H_ */
