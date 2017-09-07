#include <utils/Timer.h>
/*
 * Initializes a timer register for interrupts
 */
void initTimerInterrupt(TIM_TypeDef *timerReg, int timeDelay) {
	//TODO: implement this
}

/*
 * Intializes a timer register (TIM 2-5 for PWM generation
 * CAUTION: can run 4 different pulse widths per timer but only 1 frequency due to shared ARR regs.
 */
void initTimerPWM(TIM_TypeDef *timerReg, uint8_t channel, GPIO_TypeDef *gpio, uint8_t pin, int pulseWidthMicroSec, int frequency) {

	uint8_t alternateFunc;
	int timerTwo = (int)timerReg == (int)TIM2;
	int timerThree = (int)timerReg == (int)TIM3;
	int timerFour = (int)timerReg == (int)TIM4;
	int timerFive = (int)timerReg == (int)TIM5;

	if(timerTwo) { //sets Alternate Function to correct one according to datasheet.
			alternateFunc = 0x01;
	}
	else if(timerThree || timerFour || timerFive) {
			alternateFunc = 0x02;
	}

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	gpio->MODER |= ((uint8_t)0x2 << pin*2);
	gpio->AFR[pin/8] |= ((alternateFunc) << (pin%8)*4);

	__disable_irq();

	switch((int)timerReg) {
	//TIM 1/2 use AF01
	case (int)TIM2:
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		break;
	//TIM 3/4/5 use AF02
	case (int)TIM3:
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		break;
	case (int)TIM4:
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		break;
	case (int)TIM5:
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
		break;
	}

	timerReg->DIER |= TIM_DIER_UIE;
	timerReg->PSC = 1000-1; //1Mhz clock
	timerReg->ARR = (100000/frequency)-1;

	switch(channel) {
	case 1:
		timerReg->CCR1 = (timerReg->ARR) - pulseWidthMicroSec;
		timerReg->CCMR1 |= TIM_CCMR1_OC1M;
		break;
	case 2:
		timerReg->CCR2 = (timerReg->ARR) - pulseWidthMicroSec;
		timerReg->CCMR1 |= TIM_CCMR1_OC2M;
		break;
	case 3:
		timerReg->CCR3 = (timerReg->ARR) - pulseWidthMicroSec;
		timerReg->CCMR2 |= TIM_CCMR2_OC3M;
		break;
	case 4:
		timerReg->CCR4 = (timerReg->ARR) - pulseWidthMicroSec;
		timerReg->CCMR2 |= TIM_CCMR2_OC4M;
		break;
	}
	timerReg->CCER |= (((uint8_t)0x1) << (channel-1)*4);
	timerReg->CR1 |= TIM_CR1_CEN;

	__enable_irq();
}


