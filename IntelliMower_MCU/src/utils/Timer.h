#ifndef UTILS_TIMERUTILS_H_
#define UTILS_TIMERUTILS_H_

#include "stm32f411xe.h"
#include "GPIO.h"

typedef enum {
	PWM_GENERATION,
	INTERNAL_TIMER,
}TimerMode;

typedef struct {
	TimerMode mode;
	TIM_TypeDef *timerReg;
}Timer;

void initTimerPWM(TIM_TypeDef *timerReg, uint8_t channel, GPIO_TypeDef *gpio, uint8_t pin, int pulseWidth, int frequecy);
void timerSetPWM(TIM_TypeDef *timerReg, uint8_t channel, int pulseWidthMicroSec, int frequency);
void timerRegEnable(TIM_TypeDef *timerReg);

#endif /* UTILS_TIMERUTILS_H_ */
