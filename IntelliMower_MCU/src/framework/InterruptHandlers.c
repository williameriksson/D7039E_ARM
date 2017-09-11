/*
 * InterruptHandlers.c
 */

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

	uint8_t rpiCMD;

	if (SPI2->SR & SPI_SR_RXNE) {
		GPIOA->ODR |= GPIO_ODR_ODR_5;
		rpiCMD = SPI2->DR;
	}

	RunCommand();

}

/*
 *  In the control loop while motor pwm and pid is running.
 *  Alternatively sleep for next internal interrupt
 */
void Timer_Interrupt_For_ControlLoop( ) {
	// Enable interrupt again

}
