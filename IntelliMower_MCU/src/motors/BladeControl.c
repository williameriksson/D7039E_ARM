#include "BladeControl.h"

void InitBladeControl() {
	GpioEnable(GPIOA);
	GpioSetOutput(GPIOA, 1);
	GpioSetPinHigh(GPIOA, 1);
}

void StartBlades() {
	GpioSetPinLow(GPIOA, 1);
}

void StopBlades() {
	GpioSetPinHigh(GPIOA, 1);
}

