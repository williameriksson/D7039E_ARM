#include "Encoder.h"


int ccrReg;
int ccrPrevReg;
int countReg;
int leftIntCount;
int rightIntCount;

double leftAngle;
double rightAngle;
double angle;

double GetPosAngle() {
	return posAngle;
}

void PositionCal(Point *p, double deltaDistTraveled, double angleDeg) {
    double angleRad = angleDeg * M_PI / 180.0;
    p->x = p->x + deltaDistTraveled * cos(angleRad);
    p->y = p->y + deltaDistTraveled * sin(angleRad);
}

void InitEncoders() {

	ccrReg = 0;
	ccrPrevReg = 0;
	countReg = 0;
	angle = 0.0;
	ResetEncoder(&leftEncoder);
	ResetEncoder(&rightEncoder);
	leftEncoder.encoderDistance = 0.0f;
	rightEncoder.encoderDistance = 0.0f;
	leftAngle = 0.0;
	rightAngle = 0.0;

//	xxPos = 0.0;
//	yyPos = 0.0;
	posAngle = 0.0;

	currentPosition.x = 0.0;
	currentPosition.y = 0.0;

	GpioEnable(GPIOA);
//	GpioEnable(GPIOB);
	GpioEnable(GPIOC);

	GpioSetOutput(GPIOC, 8);
	GpioSetOutput(GPIOC, 9);
	GpioSetAF(GPIOB, 6, 2); //PW to determine speed (PB6)
	GpioSetAF(GPIOB, 7, 2); //PW to determine speed (PB8) (PB8 for nucleo, should be PB7)

//	GpioSetAF(GPIOB, 5, 2);

	GpioSetInput(GPIOB, 4); //input to determine the direction (1 or 0) (PB4)
	GpioSetPupdr(GPIOB, 4, DISABLED);
	GpioSetInput(GPIOB, 5); //input to determine the direction (1 or 0) (PB5)
	GpioSetPupdr(GPIOB, 5, DISABLED);

	TimerRegEnable(TIM4);
	TIM4->PSC = 4096-1; //sets the register
	TIM4->ARR = 0xFFFF;
	TIM4->CR1 |= TIM_CR1_CEN;
//	TIM4->DIER |= TIM_DIER_UIE; //update on overflow (for timeout checking)

	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0; //ch1 set to input capture, CCR read-only.
	TIM4->CCMR1 |= TIM_CCMR1_CC2S_0; //ch2 set to input capture, CCR read-only.

	TIM4->CCER |= TIM_CCER_CC1P; //force to 1, rising and falling edge
	TIM4->CCER |= TIM_CCER_CC1NP; //force to 1,

	TIM4->CCER |= TIM_CCER_CC2P; //force to 1, rising and falling edge
	TIM4->CCER |= TIM_CCER_CC2NP; //force to 1,

	TIM4->DIER |= TIM_DIER_CC1IE; //interrupt on event 1
	TIM4->DIER |= TIM_DIER_CC2IE; //interrupt on event 2

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
		return (-1)*speed;
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
//		GpioSetPinToggle(GPIOC, 8);
//		if(TIM4->CNT - rightEncoder.prevReg < 500) {
//			int trash = TIM4->CCR1;
//			return;
//		}
		rightIntCount++;
		rightEncoder.reg = TIM4->CCR1;
		TIM4->SR &= ~TIM_SR_CC1IF;
		ccrReg = rightEncoder.reg;
		ccrPrevReg = rightEncoder.prevReg;
		countReg = TIM4->CNT;
		int direction = GpioReadInputPin(GPIOB, 4);
		rightEncoder.encoderSpeed = GetEncoderSpeed(rightEncoder.reg, rightEncoder.prevReg, 1); //TODO: fix this
		rightEncoder.prevReg = rightEncoder.reg;
		rightEncoder.encoderDistance += 75.4 / 16.0 * (rightEncoder.encoderSpeed / abs(rightEncoder.encoderSpeed));
		rightEncoder.timeout = 0; //this encoder has interrupted this counter-lapse
		rightAngle += 22.5;
		if(GetRightSpeed() >= 0) {
			//xxPos += 0.11046 * cos(posAngle / 180.0 * M_PI) - 2.3475 * sin(posAngle / 180.0 * M_PI);
			//yyPos += 0.11046 * sin(posAngle / 180.0 * M_PI) + 2.3475 * cos(posAngle / 180.0 * M_PI);
			posAngle -= 5.388;
			PositionCal(&currentPosition, -2.3501, posAngle);
		}
		else {
			//xxPos -= 0.11046 * cos(posAngle / 180.0 * M_PI) - 2.3475 * sin(posAngle / 180.0 * M_PI);
			//yyPos -= 0.11046 * sin(posAngle / 180.0 * M_PI) + 2.3475 * cos(posAngle / 180.0 * M_PI);
			posAngle += 5.388;
			PositionCal(&currentPosition, 2.3501, posAngle);
		}
//		if(_state != TURNING) {
//		}
	}
	else if(TIM4->SR & TIM_SR_CC2IF) { //Encoder 2 rising edge
//		GpioSetPinToggle(GPIOC, 9);
//		if(TIM4->CNT - leftEncoder.prevReg < 500) {
//			int trash = TIM4->CCR2;
//			return;
//		}
		leftIntCount++;
		leftEncoder.reg = TIM4->CCR2;
		TIM4->SR &= ~TIM_SR_CC2IF;
		ccrReg = leftEncoder.reg;
		ccrPrevReg = leftEncoder.prevReg;
		countReg = TIM4->CNT;
		int direction = GpioReadInputPin(GPIOB, 4);
		leftEncoder.encoderSpeed = GetEncoderSpeed(leftEncoder.reg, leftEncoder.prevReg, 1); //TODO: fix this
		leftEncoder.prevReg = leftEncoder.reg;
		leftEncoder.encoderDistance += 75.4 / 16.0 * (leftEncoder.encoderSpeed / abs(leftEncoder.encoderSpeed));
		leftEncoder.timeout = 0; //this encoder has interrupted this counter-lapse
		leftAngle += 22.5;

		if(GetLeftSpeed() >= 0) {
			posAngle += 5.388; //for every turn of a wheel, the mower rotates this amount of degrees
			PositionCal(&currentPosition, -2.3501, posAngle);
		}
		else {
			posAngle -= 5.388;
			PositionCal(&currentPosition, 2.3501, posAngle);
		}
//		if(_state != TURNING) {
//		}
	}
	else { //Timer overflow.
		if(leftEncoder.timeout == 1) { //encoders has not interrupted this lapse, assume timeout.
//			ResetEncoder(&leftEncoder);
		}
		else {
			leftEncoder.timeout = 1;
		}
		if(rightEncoder.timeout == 1) { //encoders has not interrupted this lapse, assume timeout.
//			ResetEncoder(&rightEncoder);
		}
		else {
			rightEncoder.timeout = 1;
		}
	}
	angle = ((leftAngle + rightAngle)*12.0) / (2.0 * 25.0);
}
