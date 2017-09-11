#include <utils/Timer.h>
/*
 * Initializes a timer register for generating interrupts after timeDelay
 * timeDelay is in milli seconds.
 * CAUTION: using delays longer than 2^16 ms requires a 32bit timer (TIM2 or TIM5)
 */
void InitTimerInterrupt(TIM_TypeDef *timerReg, int timeDelay) {
	TimerRegEnable(timerReg);
	timerReg->DIER |= TIM_DIER_UIE; //enables update interrupts
	timerReg->PSC = 10000-1; //sets prescaler -> clock freq 10 kHz
	timerReg->ARR = timeDelay*10; //timedelay is MS delay
	timerReg->CR1 |= TIM_CR1_CEN; //enables the Timer.

	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 20);
}

/*
 * Intializes a timer register (TIM 1-5 / 9-11 for PWM generation)
 * CAUTION: can run 4 / 2 different pulse widths per timer but only 1 frequency due to shared ARR regs.
 */
void InitTimerPWM(TIM_TypeDef *timerReg, uint8_t channel, GPIO_TypeDef *gpio, uint8_t pin, int pulseWidthMicroSec, int frequency) {

	uint8_t alternateFunc;
	int timerOne = (int)timerReg == (int)TIM1;
	int timerTwo = (int)timerReg == (int)TIM2;
	int timerThree = (int)timerReg == (int)TIM3;
	int timerFour = (int)timerReg == (int)TIM4;
	int timerFive = (int)timerReg == (int)TIM5;

	int timerNine = (int)timerReg == (int)TIM9;
	int timerTen = (int)timerReg == (int)TIM10;
	int timerEleven = (int)timerReg == (int)TIM11;

	if (timerOne || timerTwo) { //sets Alternate Function to correct one according to datasheet.
		alternateFunc = 0x01;
	}
	else if (timerThree || timerFour || timerFive) {
		alternateFunc = 0x02;
	}
	else if (timerNine || timerTen || timerEleven) {
		alternateFunc = 0x03;
	}

	GpioEnable(gpio);
	GpioSetAF(gpio, pin, alternateFunc);

	__disable_irq();

	TimerRegEnable(timerReg); //enables the timerReg register

	TimerSetPWM(timerReg, channel, pulseWidthMicroSec, frequency); //sets the actual PWM modulation

	timerReg->CCER |= (((uint8_t)0x1) << (channel-1)*4);
	timerReg->CR1 |= TIM_CR1_CEN;

	__enable_irq();
}

/*
 * sets the PWM width and frequency for specified timer and channel.
 */
void TimerSetPWM(TIM_TypeDef *timerReg, uint8_t channel, int pulseWidthMicroSec, int frequency) {

	int pulseWidth = pulseWidthMicroSec;
	int timerOne = (int)timerReg == (int)TIM1;
	int timerTwo = (int)timerReg == (int)TIM2;
	int timerFive = (int)timerReg == (int)TIM5;

	if(timerOne || timerTwo || timerFive) {
			//32 bit timers, increased resolution
			timerReg->DIER |= TIM_DIER_UIE; //Update interrupts enabled
			timerReg->PSC = 100-1; //1 Mhz clock
			timerReg->ARR = (1000000/frequency)-1;
		}
	else {
		//16 bit timers, decreased resolution
		timerReg->DIER |= TIM_DIER_UIE; //Update interrupts enabled
		timerReg->PSC = 10000-1; //10 khz clock
		timerReg->ARR = (10000/frequency)-1;
		pulseWidth = pulseWidth / 100;
	}

	switch(channel) {
	case 1:
		timerReg->CCR1 = (timerReg->ARR) - pulseWidth;
		timerReg->CCMR1 |= TIM_CCMR1_OC1M;
		break;
	case 2:
		timerReg->CCR2 = (timerReg->ARR) - pulseWidth;
		timerReg->CCMR1 |= TIM_CCMR1_OC2M;
		break;
	case 3:
		timerReg->CCR3 = (timerReg->ARR) - pulseWidth;
		timerReg->CCMR2 |= TIM_CCMR2_OC3M;
		break;
	case 4:
		timerReg->CCR4 = (timerReg->ARR) - pulseWidth;
		timerReg->CCMR2 |= TIM_CCMR2_OC4M;
		break;
	}
}

void TimerRegEnable(TIM_TypeDef *timerReg) {
	switch((int)timerReg) {
		//TIM 1/2 is enabled in APB2ENR / APB1ENR reg
		case (int)TIM1:
			RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
			break;
		case (int)TIM2:
			RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
			break;
		//TIM 3/4/5 is enabled in APB1ENR reg
		case (int)TIM3:
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
			break;
		case (int)TIM4:
			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
			break;
		case (int)TIM5:
			RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
			break;
		//TIM 9/10/11 is enabled in APB2ENR reg
		case (int)TIM9:
			RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
			break;
		case (int)TIM10:
			RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
			break;
		case (int)TIM11:
			RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
			break;
		}
}




