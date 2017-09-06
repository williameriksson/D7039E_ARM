#include "motorControl.h"

void initMotorControl() {
	initTimerPWM(TIM2, 3, GPIOB, 10, 1500, 50);
}
