#include "UltraSonic.h"


void InitUltrasonic() {
//	__disable_irq();
//	__enable_irq();

	InitTimerPWM(TIM2, 1, GPIOA, 5, 40000, 2); //Should be changed for TIM9 ch 1 (PA2) (is tim2 due to lack of support from nucleo)

	/*
	 * sets up a timer for capturing input from ultrasonic sensors.
	 */
	GpioEnable(GPIOA);
	GpioSetAF(GPIOA, 6, 2);

	TimerRegEnable(TIM3);
	TIM3->PSC = 10000-1; //clock scaled to 10kHz
	TIM3->ARR = 0xFFFF; // Auto reload at max
	TIM3->CR1 |= TIM_CR1_CEN; // Enable timer

	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0; //ch1 set to input capture, CCR read-only.
	//no filter set (assumed ultrasonic output to be stable)
	TIM3->CCER &= ~TIM_CCER_CC1P; //force to 0, normal polarity
	TIM3->CCER &= ~TIM_CCER_CC1NP; //force to 0,

//	TIM3->CCER |= TIM_CCER_CC1P; //force to 1
//	TIM3->CCER |= TIM_CCER_CC1NP; //force to 1
	TIM3->SMCR |= TIM_SMCR_SMS_2;

	TIM3->CCMR1 &= ~TIM_CCMR1_IC1PSC; //psc disabled, captures at every valid transition
	TIM3->CCER |= TIM_CCER_CC1E; //enables the TIM3 channel 1 (PA06)


//	TIM10->PSC = 10000-1; // Prescale to 10kHz
//	TIM10->ARR = 0xFFFF; // Auto reload at max
//	TIM10->CR1 |= TIM_CR1_CEN; // Enable TIM10

//	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock

//	GpioEnable(GPIOA);
//	GpioSetAF(GPIOB, 6, 2);
//
//	GpioEnable(GPIOC);

//	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PC; // Set external interrupt EXTI4 for PC4
//
//	EXTI->RTSR |= EXTI_RTSR_TR4; // Enable interrupt on rising edge for TR4
//	EXTI->FTSR |= EXTI_FTSR_TR4; // Enable interrupt on falling edge for TR4
//	EXTI->IMR |= EXTI_IMR_MR4; // Unmask the interrupt register for MR4 (Active for PC4)
//
//	GPIOA->MODER |= GPIO_MODER_MODER8_0;
//	NVIC_SetPriority(EXTI4_IRQn, 23); // Set the priority, this should probably be changed..
//	NVIC_EnableIRQ(EXTI4_IRQn); // Enable the interrupt
}

//void EXTI4_IRQHandler (void) {
//	static int startTime = 0;
//
//	if (EXTI->PR & EXTI_PR_PR4) {	// Check interrupt flag for PR4
//		if (GPIOC->IDR & 16) {		// Check if rising edge
//			startTime = TIM10->CNT;
//		} else {
//			int endTime = TIM10->CNT;
//			int duration = endTime - startTime;
//			if (duration < 0) {
//				duration = (0xFFFF - startTime) + endTime;
//			}
//			pushBuffer(&obstacleBuffer, duration);
//			int averageDuration = getBufferAverage(&obstacleBuffer);
//			obstacleDistance = ((float)averageDuration / 1.47f) * 2.54f; // 147 uS per cm
//			if (obstacleDistance <= obstacleThreshold) {
//				GPIOA->ODR |= (1 << 8);
//			} else {
//				GPIOA->ODR &= ~(1 << 8);
//			}
//		}
//	}
//	EXTI->PR |= EXTI_PR_PR4; // clear interrupt flag PR4 by writing 1
//}

//void TIM1_BRK_TIM9_IRQHandler() {
//	TIM9->SR &= ~TIM_SR_UIF;
//	TIM9->SR &= ~TIM_SR_CC1IF;
//	GpioSetPinToggle(GPIOA, 3);
//}
