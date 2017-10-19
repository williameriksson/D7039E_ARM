#ifndef MOTORS_MOTORCONTROL_H_
#define MOTORS_MOTORCONTROL_H_

#include "stm32f411xe.h"
#include "utils/Timer.h"
#include "utils/GPIO.h"

/*
 * Struct containing immuatable information regarding controlling a motor.
 */
typedef struct {
	TIM_TypeDef *timer;
	GPIO_TypeDef *gpio;
	const int channel; //TIM channel for PWM generation
	const int pin; //output pin for PWM
	const int pwmMin; //minimum PWM width in micro sec
	const int pwmMax; //maximum PWM width in micro sec
	int frequency; //in Hz
	int pulseWidth; //in millisec
	int currentSpeed; //in percentage
}MotorController;

void InitMotorControl();
void SetMotorSpeed(MotorController *motorController, int percentage);
int GetMotorSpeed();

#endif /* MOTORS_MOTORCONTROL_H_ */
