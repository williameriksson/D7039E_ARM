/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "framework/Common.h"


/*
 *
 */
int main( void ) {
	//sysInit();

	// Place hardware or system inits
	// here that are not initialized in common init

	CommonInit();  // master init
	CommonFrame(); // busy wait while loop
}
