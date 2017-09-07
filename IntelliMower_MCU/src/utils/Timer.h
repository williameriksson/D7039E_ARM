#ifndef UTILS_TIMERUTILS_H_
#define UTILS_TIMERUTILS_H_

#include "stm32f411xe.h"

//void initTimer(TIM_TypeDef*);

void initTimerPWM(TIM_TypeDef *timerReg, uint8_t channel, GPIO_TypeDef *gpio, uint8_t pin, int pulseWidth, int frequecy);


#endif /* UTILS_TIMERUTILS_H_ */
