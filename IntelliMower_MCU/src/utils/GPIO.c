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

void GpioSetOutSpeed(GPIO_TypeDef *gpio, int pin, GPIO_O_SPEED speed) {
	gpio->OSPEEDR |= ((uint8_t)speed << pin*2);
}

void GpioSetOtyper(GPIO_TypeDef *gpio, int pin) {
	gpio->OTYPER |= ((uint8_t)0x1 << pin);
}

void GpioSetPupdr(GPIO_TypeDef *gpio, int pin, GPIO_PUPDR mode) {
	gpio->PUPDR &= ~((uint8_t)0x3 << pin*2);
	gpio->PUPDR |= ((uint8_t)mode << pin*2);
}

int GpioReadInputPin(GPIO_TypeDef *gpio, int pin) {
	int temp = gpio->IDR;
	return (temp >> pin) & 0x1;
}
