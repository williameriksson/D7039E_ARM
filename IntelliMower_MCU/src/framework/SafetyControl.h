#ifndef FRAMEWORK_SAFETYCONTROL_H_
#define FRAMEWORK_SAFETYCONTROL_H_

#include "ControlLoop.h"
#include "motors/Steering.h"
#include "sensors/UltraSonic.h"
#include "utils/Timer.h"
#include "stm32f4xx_nucleo.h"

void InitSafetyControl();
int IsObjectInFront();

#endif /* FRAMEWORK_SAFETYCONTROL_H_ */
