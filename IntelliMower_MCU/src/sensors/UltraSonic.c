#include "UltraSonic.h"

void InitUltrasonic() {
//	__disable_irq();
//	__enable_irq();
	distanceSensors.distances[0] = 0;
	distanceSensors.distances[1] = 0;
	distanceSensors.distances[2] = 0;
	distanceSensors.distances[3] = 0;
 	InitTimerPWM(TIM2, 1, GPIOA, 5, 40000, 2); //Should be changed for TIM9 ch 1 (PA2) (is tim2 due to lack of support from nucleo)

	/*
	 * sets up a timer for capturing input from ultrasonic sensors.
	 */
	GpioEnable(GPIOA);
	GpioSetAF(GPIOA, 6, 2);
	GpioSetAF(GPIOA, 7, 2);

	TimerRegEnable(TIM3);
	TIM3->PSC = 10000-1; //clock scaled to 10kHz
	TIM3->ARR = 0xFFFF; // Auto reload at max
	TIM3->CR1 |= TIM_CR1_CEN; // Enable timer

	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0; //ch1 set to input capture, CCR read-only.
	TIM3->CCMR1 |= TIM_CCMR1_CC2S_0; //ch2 set to input capture, CCR read-only.

	//no filter set (assumed ultrasonic output to be stable)
//	TIM3->CCER &= ~TIM_CCER_CC1P; //force to 0, normal polarity
//	TIM3->CCER &= ~TIM_CCER_CC1NP; //force to 0,

	TIM3->CCER |= TIM_CCER_CC1P; //force to 1, both edges
	TIM3->CCER |= TIM_CCER_CC1NP; //force to 1,

	TIM3->CCER |= TIM_CCER_CC2P; //force to 1, both edges
	TIM3->CCER |= TIM_CCER_CC2NP; //force to 1,

	TIM3->DIER |= TIM_DIER_CC1IE;
	TIM3->DIER |= TIM_DIER_CC2IE;

//	TIM3->CCER |= TIM_CCER_CC2P; //
//	TIM3->CCER &= ~TIM_CCER_CC2NP; //active on falling edge

//	TIM3->SMCR |= TIM_SMCR_TS_0;
//	TIM3->SMCR |= TIM_SMCR_TS_2; //TS bits to 101 (TI1FP1 selected)

//	TIM3->SMCR |= TIM_SMCR_SMS_2; //SMS bits to 100 (slave mode controller in reset mode)

	TIM3->CCMR1 &= ~TIM_CCMR1_IC1PSC; //psc disabled, captures at every valid transition
	TIM3->CCER |= TIM_CCER_CC1E; //enables the TIM3 channel 1 (PA06)
	TIM3->CCER |= TIM_CCER_CC2E; //enables the TIM3 channel 2 (PA06)

	NVIC_SetPriority(TIM3_IRQn, 23); // Set the priority, this should probably be changed..
	NVIC_EnableIRQ(TIM3_IRQn); // Enable the interrupt

	GpioEnable(GPIOB);
	GpioSetOutput(GPIOB, 6);
}

int ccrOne; //for testing in STM viewier.
int ccrTwo;


//void TIM3_IRQHandler() {
//	TIM3->SR &= ~TIM_SR_UIF;
//	int ccr;
//	int* write;
////	int fallingEdge = 0;
//	if(TIM3->SR & TIM_SR_CC1IF) {
//		ccr = TIM3->CCR1;
//		write = &distanceSensors.distances[0];
//	}
//	else if(TIM3->SR & TIM_SR_CC2IF) {
//		ccr = TIM3->CCR2;
//		write = &distanceSensors.distances[1];
//	}
//	else if(TIM3->SR & TIM_SR_CC3IF) {
//		ccr = TIM3->CCR3;
//		write = &distanceSensors.distances[2];
//	}
//	else if(TIM3->SR & TIM_SR_CC4IF) {
//		ccr = TIM3->CCR4;
//		write = &distanceSensors.distances[3];
//	}
//	if(ccr <= 250) { //TODO: do this in a better way preferably.
//		*write = ccr;
//	}
//	else {
//	}
//	TIM3->CNT = 0;
//}
