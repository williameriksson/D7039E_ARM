#include "MotorControl.h"

void initMotorControl() {
	//sets PWM gen. TIM9 ch2, PA3 1.5ms 50hz
	initTimerPWM(TIM9, 2, GPIOA, 3, 1500, 50);
}

