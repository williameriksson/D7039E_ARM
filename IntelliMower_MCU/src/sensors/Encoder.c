#include "Encoder.h"

RotaryEncoder leftEncoder;
RotaryEncoder rightEncoder;

void InitEncoders() {

	ResetEncoder(&leftEncoder);
	ResetEncoder(&rightEncoder);

	GpioEnable(GPIOB);
	GpioSetAF(GPIOB, 6, 2);
	GpioSetAF(GPIOB, 7, 2);

	GpioSetInput(GPIOB, 4); //input to determine the direction (1 or 0) (PB4)
	GpioSetInput(GPIOB, 5); //input to determine the direction (1 or 0) (PB5)

	TimerRegEnable(TIM4);
	TIM4->PSC = 4096-1; //sets the register
	TIM4->ARR = 0xFFFF;
	TIM4->CR1 |= TIM_CR1_CEN;
	TIM4->DIER |= TIM_DIER_UIE; //update on overflow (for timeout checking)

	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0; //ch1 set to input capture, CCR read-only.
	TIM4->CCMR1 |= TIM_CCMR1_CC2S_0; //ch2 set to input capture, CCR read-only.

	TIM4->CCER &= ~TIM_CCER_CC1P; //force to 0, rising edge
	TIM4->CCER &= ~TIM_CCER_CC1NP; //force to 0,
	TIM4->CCER &= ~TIM_CCER_CC2P; //force to 0, rising edge
	TIM4->CCER &= ~TIM_CCER_CC2NP; //force to 0,

	TIM4->DIER |= TIM_DIER_CC1IE; //interrupt on rising edge
	TIM4->DIER |= TIM_DIER_CC2IE; //interrupt on rising edge

	TIM4->CCER |= TIM_CCER_CC1E; //enables the TIM4 channel 1 (PB06)
	TIM4->CCER |= TIM_CCER_CC2E; //enables the TIM4 channel 2 (PB07)

	NVIC_SetPriority(TIM4_IRQn, 20); // Set the priority, this should probably be changed..
	NVIC_EnableIRQ(TIM4_IRQn); // Enable the interrupt

}

int GetEncoderSpeed(int reg, int prevReg, int direction) {
	int speed = reg - prevReg;
	if(reg < prevReg) {
		speed += 65535;
	}

	if(direction == 0) {
		return -speed;
	}
	else {
		return speed;
	}
}

void ResetEncoder(RotaryEncoder *encoder) {
	encoder->encoderSpeed = 0;
	encoder->prevReg = 0;
	encoder->reg = 0;
	encoder->timeout = 1;
}

void TIM4_IRQHandler() {
	TIM4->SR &= ~TIM_SR_UIF;

	if(TIM4->SR & TIM_SR_CC1IF) { //Encoder 1 rising edge
		rightEncoder.reg = TIM4->CCR1;
		int direction = GpioReadInputPin(GPIOB, 4);
		rightEncoder.encoderSpeed = GetEncoderSpeed(rightEncoder.reg, rightEncoder.prevReg, direction);
		rightEncoder.prevReg = rightEncoder.reg;
		rightEncoder.timeout = 0; //this encoder has interrupted this counter-lapse
	}
	else if(TIM4->SR & TIM_SR_CC2IF) { //Encoder 2 rising edge
		leftEncoder.reg = TIM4->CCR2;
		int direction = GpioReadInputPin(GPIOB, 5);
		leftEncoder.encoderSpeed = GetEncoderSpeed(leftEncoder.reg, leftEncoder.prevReg, direction);
		leftEncoder.prevReg = leftEncoder.reg;
		leftEncoder.timeout = 0; //this encoder has interrupted this counter-lapse
	}
	else { //Timer overflow.
		if(leftEncoder.timeout == 1) { //encoders has not interrupted this lapse, assume timeout.
			ResetEncoder(&leftEncoder);
		}
		else {
			leftEncoder.timeout = 1;
		}
		if(rightEncoder.timeout == 1) { //encoders has not interrupted this lapse, assume timeout.
			ResetEncoder(&rightEncoder);
		}
		else {
			rightEncoder.timeout = 1;
		}
	}
}
