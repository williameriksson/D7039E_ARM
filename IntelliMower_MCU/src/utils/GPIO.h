#ifndef UTILS_GPIO_H_
#define UTILS_GPIO_H_

#include "stm32f411xe.h"

void gpioEnable(GPIO_TypeDef *gpio);
void gpioSetInput(GPIO_TypeDef *gpio, int pin);
void gpioSetOutput(GPIO_TypeDef *gpio, int pin);
void gpioSetAF(GPIO_TypeDef *gpio, int pin, uint8_t altFunc);
void gpioSetAnalog(GPIO_TypeDef *gpio, int pin);
void gpioSetPinHigh(GPIO_TypeDef *gpio, int pin);
void gpioSetPinLow(GPIO_TypeDef *gpio, int pin);

#endif /* UTILS_GPIO_H_ */
