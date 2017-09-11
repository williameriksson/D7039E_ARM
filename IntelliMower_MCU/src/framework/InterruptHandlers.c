/*
 * InterruptHandlers.c
 */
#include "stm32f411xe.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"

#include "InterruptHandlers.h"
#include "CmdSystem.h"


/*
 * Name is pseudo code
 */
void Usonic_InterruptHandler () {
	// Enable interrupt again

}

/*
 *  Used for SPI commands from raspberry
 */
void SPI2_IRQHandler (void) {

	rpiCMD_t *newCMD = MCU_NULL;

	if (SPI2->SR & SPI_SR_RXNE) {
		GPIOA->ODR |= GPIO_ODR_ODR_5;
		*newCMD = SPI2->DR;
	}

	if ( !RunCommand( newCMD ) ) {	// catch errors or if no cmd was sent to run cmd

	}

}

/*
 *  In the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 */
void Timer_Interrupt_For_ControlLoop( ) {
	// Enable interrupt again

}
