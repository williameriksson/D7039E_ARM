#ifndef FRAMEWORK_SAFETYCONTROL_H_
#define FRAMEWORK_SAFETYCONTROL_H_

#include "ControlLoop.h"
#include "motors/Steering.h"
#include "sensors/UltraSonic.h"
#include "utils/Timer.h"
#include "stm32f4xx_nucleo.h"

void InitSafetyControl();
int IsObjectInFront();
void HaltOperation();
void ResumeOperation();

int BumperFront; 	//on PB02
int LiftSensor;		//on PC0
int BumperBack; 	//on PC05

#endif /* FRAMEWORK_SAFETYCONTROL_H_ */
