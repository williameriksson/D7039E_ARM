#include "SafetyControl.h"

const int objectDistanceMargin = 40;

void InitSafetyControl() {
	InitTimerInterrupt(TIM11, 100, 10); //10hz loop
	GpioEnable(GPIOB);
	GpioEnable(GPIOC);

	GpioSetInput(GPIOB, 2);
	GpioSetInput(GPIOC, 0);
	GpioSetInput(GPIOC, 5);
}

int prevSpeedLeft;
int prevSpeedRight;

void TIM1_TRG_COM_TIM11_IRQHandler() {
//	BumperFront = 0x4 & GPIOB->IDR; 	//on PB02
//	LiftSensor = 0x1 & GPIOC->IDR;			//on PC0
//	BumperBack = 0x32 & GPIOC->IDR; 	//on PC05
	BumperFront = GpioReadInputPin(GPIOB, 2);
	LiftSensor = GpioReadInputPin(GPIOC, 0);
	BumperBack = GpioReadInputPin(GPIOC, 5);

//	if(IsObjectInFront() || LiftSensor || BumperFront || BumperBack) {
	if(LiftSensor || BumperFront || BumperBack ) {
//		HaltOperation();
	}
	else {
//		DriveForward(-2000);
	}
}

void HaltOperation() {
	prevSpeedLeft = GetLeftSpeed();
	prevSpeedRight = GetRightSpeed();
	StopController(); //stops the PID (if it's running)
	DriveForward(0); //come to a complete stop immediately.
	//Also stop blades
}

void ResumeOperation() {

}

int IsObjectInFront() {
	for(int i = 0; i < 4; i++) {
		if(0) {
			return 1;
		}
	}
	return 0;
}

