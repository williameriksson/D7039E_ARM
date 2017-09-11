#include "GPIO.h"

void GpioEnable(GPIO_TypeDef *gpio) {
	switch((int)gpio) {
		case (int)GPIOA:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
			break;
		case (int)GPIOB:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
			break;
		case (int)GPIOC:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
			break;
		case (int)GPIOD:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
			break;
		case (int)GPIOE:
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
			break;
		}
}

//sets gpio on pin to input mode (default mode)
void GpioSetInput(GPIO_TypeDef *gpio, int pin) {
	gpio->MODER &= ~((uint8_t)0x3 << pin*2);
}

//sets gpio on pin to general purpose output mode
void GpioSetOutput(GPIO_TypeDef *gpio, int pin) {
	gpio->MODER |= ((uint8_t)0x1 << pin*2);
}

//sets gpio on pin to alternate function mode and sets the alt func.
void GpioSetAF(GPIO_TypeDef *gpio, int pin, uint8_t altFunc) {
	gpio->MODER |= ((uint8_t)0x2 << pin*2);
	gpio->AFR[pin/8] |= ((altFunc) << (pin%8)*4);
}

//sets gpio on pin to analog mode
void GpioSetAnalog(GPIO_TypeDef *gpio, int pin) {
	gpio->MODER &= ((uint8_t)0x3 << pin*2);
}

void GpioSetPinHigh(GPIO_TypeDef *gpio, int pin) {
	gpio->ODR |= ((uint8_t)0x1 << pin);
}

void GpioSetPinLow(GPIO_TypeDef *gpio, int pin) {
	gpio->ODR &= ~((uint8_t)0x1 << pin);
}

void GpioSetPinToggle(GPIO_TypeDef *gpio, int pin) {
	gpio->ODR ^= ((uint8_t)0x1 << pin);
}
