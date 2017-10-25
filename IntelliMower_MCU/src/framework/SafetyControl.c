#include "SafetyControl.h"

const int objectDistanceMargin = 40;

void InitSafetyControl() {
	InitTimerInterrupt(TIM11, 100, 10); //10hz loop
	GpioEnable(GPIOB);
	GpioEnable(GPIOC);

	GpioSetInput(GPIOB, 2);
	GpioSetInput(GPIOC, 2);
	GpioSetInput(GPIOC, 5);
}

void TIM1_TRG_COM_TIM11_IRQHandler() {
	int BumperFront = 0x1 | (GPIOB->IDR >> 2); 	//on PB02
	int LiftSensor = 0x1 | GPIOC->IDR;			//on PC0
	int BumperBack = 0x1 | (GPIOC->IDR >> 5); 	//on PC05

	if(IsObjectInFront() || LiftSensor || BumperFront || BumperBack) {
		StopController(); //stops the PID (if it's running)
		DriveForward(0); //come to a complete stop immediately.
		//Also stop blades
	}

}

int IsObjectInFront() {
	for(int i = 0; i < 4; i++) {
		if(distanceSensors.distances[i] < objectDistanceMargin) {
			return 1;
		}
	}
	return 0;
}

