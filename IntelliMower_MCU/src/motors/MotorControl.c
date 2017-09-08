#include "MotorControl.h"

void initMotorControl() {
	//sets PWM gen. TIM9 ch2, PA3 5ms 50hz
//	initTimerPWM(TIM9, 2, GPIOA, 3, 1500, 50);

//	initTimerPWM(TIM2, 1, GPIOA, 5, 1500, 50);

	//Set TIM2 to interrupt every 1000ms.
//	initTimerInterrupt(TIM2, 1000);
}


void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	gpioEnable(GPIOA);
	gpioSetPinHigh(GPIOA, 5);
}
