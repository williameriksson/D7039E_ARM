#include "ADC.h"
#include "GPIO.h"
#include "stm32f411xe.h"


volatile int uhADC1ConvertedValue;
volatile float ADCval;

void InitLED(){
	// GPIOA and PA5 LED, AHB1
	// Before using a peripheral you have to enable its clock in the
	// RCC_AHBxENR or RCC_APBxENR register.
	// 6.3.9
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// Errata: Delay after an RCC peripheral clock enabling
	__NOP(); __NOP();
	// PA5
	// 01: General Purpose Output Mode
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	// High Speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5_0 | GPIO_OSPEEDER_OSPEEDR5_1;
	// Pull up
	//GPIOE->PUPDR |= GPIO_PUPDR_PUPDR9_0;
	// Drive Pin High
	GPIOA->BSRR |= GPIO_BSRR_BS_5;
}

void InitADC() {

    /*
    1.  Select ADC1_IN16 or ADC1_IN18 input channel.
    2.  Select a sampling time greater than the minimum sampling time specified in the
    datasheet.
    3.  Start the ADC conversion by setting the SWSTART bit (or by external trigger)
    4.  Read the resulting val in the ADC data register
    */

    // Enable the ADC Periph clock
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    __NOP(); __NOP();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	//Enables clock for GPIOC
    ADC->CCR |= ADC_CCR_ADCPRE;				//0b11;  //ADC clock 12.5MHz (100MHz prescaler 8)
    GPIOC->MODER |= GPIO_MODER_MODER4;		//Sets GPIO PC4 to analog in
	ADC1->SMPR1 |= ADC_SMPR1_SMP14;			//Sets sample rate for channel 14 to (111 = 480 cycles)
	ADC1->CR1 |= ADC_CR1_EOCIE;				//Enables interrupts for EOC
	ADC1->CR2 |= ADC_CR2_ADON;  			//sets ADON bit (turns on ADC)
	ADC1->SQR3 |= 14;

    // Turn on continuous mode
    ADC1->CR2 |= ADC_CR2_CONT;

    // Set Sample Rate to 56 cycles for channel 18
    //ADC1->SMPR1 |=ADC_SMPR1_SMP18_0 | ADC_SMPR1_SMP18_1;

    // Set Channel 18 as rank 1 (5 bits wide per register * (rank - 1))
    //ADC1->SQR3 |= 18 << (5 * (1-1));




	NVIC_EnableIRQ(ADC_IRQn);			//Enables interrupt handler
	NVIC_SetPriority(ADC_IRQn, 18);

    // Turn it on
    ADC1->CR2 |= ADC_CR2_SWSTART;

}

int GetADCValue() {
    // Wait for end of conversion
    //while ((ADC1->SR & ADC_SR_EOC) != ADC_SR_EOC);
    //return (uint16_t)(ADC1->DR & 0xFFF);
    return 0;
}

void ADC_IRQHandler (void) {
	if(ADC1->SR & ADC_SR_EOC) {
		int adcReg = ADC1->DR;		//Reads data from register to variable
		ADCval = (float)adcReg / 1241.0;

		ADC1->SR &= ~ADC_SR_EOC;
	}

}


