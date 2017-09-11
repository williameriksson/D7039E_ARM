#ifndef MOTORS_MOTORCONTROL_H_
#define MOTORS_MOTORCONTROL_H_

#include "stm32f411xe.h"
#include "utils/Timer.h"
#include "utils/GPIO.h"

/*
 * Struct containing immuatable information regarding controlling a motor.
 */
typedef const struct {
	TIM_TypeDef *timer;
	const GPIO_TypeDef *gpio;
	int channel; //TIM channel for PWM generation
	int pin; //output pin for PWM
	int pwmMin; //minimum PWM width in micro sec
	int pwmMax; //maximum PWM width in micro sec
	int frequency;
}MotorController;

void InitMotorControl();

#endif /* MOTORS_MOTORCONTROL_H_ */
