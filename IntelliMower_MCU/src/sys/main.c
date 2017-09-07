/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sys.h"
#include "../framework/Common.h"


/*
 *
 */
int main(void) {
	sysInit();
	CommonInit();
	CommonLoop();
}
