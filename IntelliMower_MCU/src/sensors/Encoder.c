#include "Encoder.h"

int prevWidth;
int width;
int prevReg;
int ccrReg;
int direction;

void InitEncoders() {
	prevWidth = 0;
	width = 0;
	direction = 0;

	GpioEnable(GPIOB);
	GpioSetAF(GPIOB, 6, 2);

	TimerRegEnable(TIM4);
//	TIM4->PSC = 4096-1; //sets the register
	TIM4->PSC = 10000-1; //sets the register
	TIM4->ARR = 0xFFFF;
	TIM4->CR1 |= TIM_CR1_CEN;

	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0; //ch1 set to input capture, CCR read-only.
	TIM4->CCMR1 |= TIM_CCMR1_CC2S_0; //ch2 set to input capture, CCR read-only.

	TIM4->CCER &= ~TIM_CCER_CC1P; //force to 0, rising edge
	TIM4->CCER &= ~TIM_CCER_CC1NP; //force to 0,

	TIM4->DIER |= TIM_DIER_CC1IE; //interrupt on rising edge

	TIM4->CCER |= TIM_CCER_CC1E; //enables the TIM4 channel 1 (PB06)

	NVIC_SetPriority(TIM4_IRQn, 20); // Set the priority, this should probably be changed..
	NVIC_EnableIRQ(TIM4_IRQn); // Enable the interrupt

	GpioSetInput(GPIOB, 4); //input to determine the direction (1 or 0)
}

void TIM4_IRQHandler() {
	TIM4->SR &= ~TIM_SR_UIF;
	int reg = TIM4->CCR1;
	ccrReg = reg;

	width = reg - prevReg;

	if(reg < prevReg) {
		width += 65535;
	}

	prevReg = reg;
	direction = GpioReadInputPin(GPIOB, 4);
}
