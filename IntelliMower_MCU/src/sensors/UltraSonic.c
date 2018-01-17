#include "UltraSonic.h"

#define PwToCm(pw) (pw) * 100.0 / 58.0

int startOne;
int startTwo;
int startThree;
int startFour;


void InitUltrasonic() {
//	__disable_irq();
//	__enable_irq();
	distanceSensors.distanceOuterLeft = 100;
	distanceSensors.distanceInnerLeft = 100;
	distanceSensors.distanceOuterRight = 100;
	distanceSensors.distanceInnerRight = 100;

	startOne = 0;
	startTwo= 0;
	startThree = 0;
	startFour = 0;

 	InitTimerPWM(TIM9, 1, GPIOA, 2, 1000, 5); // Trigger timer

	/*
	 * sets up a timer for capturing input from ultrasonic sensors.
	 */
	GpioEnable(GPIOA);
	GpioEnable(GPIOB);
	GpioEnable(GPIOC);

	GpioSetOutput(GPIOC, 5);
	GpioSetPinLow(GPIOC, 5);

	GpioSetAF(GPIOA, 6, 2); // TIM3 CH1
	GpioSetAF(GPIOA, 7, 2); // TIM3 CH2
	GpioSetAF(GPIOB, 0, 2); // TIM3 CH3
	GpioSetAF(GPIOB, 1, 2); // TIM3 CH4

	TimerRegEnable(TIM3);
	TIM3->PSC = 10000-1; //clock scaled to 10kHz
	TIM3->ARR = 0xFFFF; // Auto reload at max
	TIM3->CR1 |= TIM_CR1_CEN; // Enable timer

	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0; // ch1 set to input capture, CCR read-only.
	TIM3->CCMR1 |= TIM_CCMR1_CC2S_0; // ch2 set to input capture, CCR read-only.
	TIM3->CCMR2 |= TIM_CCMR2_CC3S_0; // ch3 set to input capture, CCR read-only.
	TIM3->CCMR2 |= TIM_CCMR2_CC4S_0; // ch4 set to input capture, CCR read-only.

	//no filter set (assumed ultrasonic output to be stable)
//	TIM3->CCER &= ~TIM_CCER_CC1P; //force to 0, normal polarity
//	TIM3->CCER &= ~TIM_CCER_CC1NP; //force to 0,

	TIM3->CCER &= ~TIM_CCER_CC1P; //force to 0, rising edge
	TIM3->CCER &= ~TIM_CCER_CC1NP; //force to 0,

	TIM3->CCER &= ~TIM_CCER_CC2P; //force to 0, rising edge
	TIM3->CCER &= ~TIM_CCER_CC2NP; //force to 0,

	TIM3->CCER &= ~TIM_CCER_CC3P;
	TIM3->CCER &= ~TIM_CCER_CC3NP;

	TIM3->CCER &= ~TIM_CCER_CC4P;
	TIM3->CCER &= ~TIM_CCER_CC4NP;

	TIM3->DIER |= TIM_DIER_CC1IE;
	TIM3->DIER |= TIM_DIER_CC2IE;
	TIM3->DIER |= TIM_DIER_CC3IE;
	TIM3->DIER |= TIM_DIER_CC4IE;

//	TIM3->CCER |= TIM_CCER_CC2P; //
//	TIM3->CCER &= ~TIM_CCER_CC2NP; //active on falling edge

//	TIM3->SMCR |= TIM_SMCR_TS_0;
//	TIM3->SMCR |= TIM_SMCR_TS_2; //TS bits to 101 (TI1FP1 selected)

//	TIM3->SMCR |= TIM_SMCR_SMS_2; //SMS bits to 100 (slave mode controller in reset mode)

	TIM3->CCMR1 &= ~TIM_CCMR1_IC1PSC; //psc disabled, captures at every valid transition
	TIM3->CCER |= TIM_CCER_CC1E; // enables the TIM3 channel 1 (PA06)
	TIM3->CCER |= TIM_CCER_CC2E; // enables the TIM3 channel 2 (PA07)
	TIM3->CCER |= TIM_CCER_CC3E; // enables the TIM3 channel 2 (PB00)
	TIM3->CCER |= TIM_CCER_CC4E; // enables the TIM3 channel 2 (PB01)

	NVIC_SetPriority(TIM3_IRQn, 23);
	NVIC_EnableIRQ(TIM3_IRQn);
}

int ccrOne; //for testing in STM viewier.
int ccrTwo;
int Timer_3;

void TIM3_IRQHandler() {
	TIM3->SR &= ~TIM_SR_UIF;
	Timer_3 = TIM3->CNT;
	if(TIM3->SR & TIM_SR_CC1IF) {
		if(!(TIM3->CCER & TIM_CCER_CC1P)) { //rising edge
			startOne = TIM3->CCR1;
			TIM3->CCER |= TIM_CCER_CC1P; //sets to interrupt on falling edge
		}
		else { //falling edge
			int temp = TIM3->CCR1 - startOne;
			distanceSensors.distanceOuterRight = PwToCm(temp);
			TIM3->CCER &= ~TIM_CCER_CC1P; //going back to rising edge
		}
	}
	else if(TIM3->SR & TIM_SR_CC2IF) {
		if(!(TIM3->CCER & TIM_CCER_CC2P)) { //rising edge
			startTwo = TIM3->CCR2;
			TIM3->CCER |= TIM_CCER_CC2P; //sets to interrupt on falling edge
		}
		else { //falling edge
			int temp = TIM3->CCR2 - startTwo;
			distanceSensors.distanceInnerRight = PwToCm(temp);
			TIM3->CCER &= ~TIM_CCER_CC2P; //going back to rising edge
		}
	}
	else if(TIM3->SR & TIM_SR_CC3IF) {
		if(!(TIM3->CCER & TIM_CCER_CC3P)) { //rising edge
			startThree = TIM3->CCR3;
			TIM3->CCER |= TIM_CCER_CC3P; //sets to interrupt on falling edge
		}
		else { //falling edge
			int temp = TIM3->CCR3 - startThree;
			distanceSensors.distanceInnerLeft = PwToCm(temp);
			TIM3->CCER &= ~TIM_CCER_CC3P; //going back to rising edge
		}
	}
	else if(TIM3->SR & TIM_SR_CC4IF) {
		if(!(TIM3->CCER & TIM_CCER_CC4P)) { //rising edge
			startFour = TIM3->CCR4;
			TIM3->CCER |= TIM_CCER_CC4P; //sets to interrupt on falling edge
		}
		else { //falling edge
			int temp = TIM3->CCR4 - startFour;
			distanceSensors.distanceOuterLeft = PwToCm(temp);
			TIM3->CCER &= ~TIM_CCER_CC4P; //going back to rising edge
			TIM3->CNT = 0;
		}
	}
//	if(ccr <= 250) { //TODO: do this in a better way preferably.
//		*write = ccr;
//	}
	//else {
	//}
}
