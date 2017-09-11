#include "MotorControl.h"

/*
 * Initializes a motorcontroller
 */
void InitMotorControl(MotorController *motorController) {
	InitTimerPWM(motorController->timer,
		motorController->channel,
		motorController->gpio,
		motorController->pin,
		1500,
		motorController->frequency
	);

//	Set TIM2 to interrupt every 1000ms.
//	InitTimerInterrupt(TIM2, 100);
}

void SetMotorSpeed(MotorController *motorController, int percentage) {
	int neutralPWM = (motorController->pwmMax + motorController->pwmMin) / 2; //"neutral" PWM
	int varianceSpan =  neutralPWM - motorController->pwmMin; //max variance of PWM from "neutral"
	int newPWM = neutralPWM + ((varianceSpan * percentage)/100);
	TimerSetPWM(motorController->timer, motorController->channel, newPWM, 50); //sets the new PWM
}

int GetMotorSpeed() {
	return 0;
}

void IncreaseMotorSpeed() {

}

void DecreaseMotorSpeed() {

}

void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	GpioEnable(GPIOA);
	GpioSetOutput(GPIOA, 5);
	GpioSetPinToggle(GPIOA, 5);
}
