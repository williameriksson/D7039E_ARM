#ifndef UTILS_GPIO_H_
#define UTILS_GPIO_H_

#include "stm32f411xe.h"

typedef enum {
	LOW,
	MEDIUM,
	FAST,
	HIGH
}GPIO_O_SPEED;

void GpioEnable(GPIO_TypeDef *gpio);
void GpioSetInput(GPIO_TypeDef *gpio, int pin);
void GpioSetOutput(GPIO_TypeDef *gpio, int pin);
void GpioSetAF(GPIO_TypeDef *gpio, int pin, uint8_t altFunc);
void GpioSetAnalog(GPIO_TypeDef *gpio, int pin);
void GpioSetPinHigh(GPIO_TypeDef *gpio, int pin);
void GpioSetPinLow(GPIO_TypeDef *gpio, int pin);
void GpioSetPinToggle(GPIO_TypeDef *gpio, int pin);
void GpioSetOutSpeed(GPIO_TypeDef *gpio, int pin, GPIO_O_SPEED speed);
void GpioSetOtyper(GPIO_TypeDef *gpio, int pin);
int GpioReadInputPin(GPIO_TypeDef *gpio, int pin);

#endif /* UTILS_GPIO_H_ */
