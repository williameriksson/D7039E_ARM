/*
 * Common.c
 */

#include "Common.h"
#include "CmdSystem.h"
#include "EventLoop.h"
#include "KeyInput.h"
#include "ControlLoop.h"

//static bool isInitialized;

/*
 *
 */
void CommonInit() {

	//isInitialized = True;
}


/*
 * Most of the Mowers state machine is handled in this function
 */
void CommonFrame() {

	// Examples of states for the state machine we could create a struct or enums for these as well
	// static bool isInitialized;
	// static bool isInIdle;
	// static bool isInClippingMode;


/*
  	pseudo code for the state machine

	 sensor = ReadSensors();
	 if (!isInIdle && ){
		 if ( sensor ) {
			Alert Raspberry
			Do Something Based on sensor reading or wait for cmd from raspberry

		 } else {
			Do normal operation
				oldEvent = newEvent;
				newEvent = PumpEvents();

				case : event {


				}

		 }
	 }
*/


}


/*
 *
 */
void CommonLoop() {

	while(1){
		/*if (!isInitialized){
			// write error code to some memory address used for errors.
			break;
		}*/
		CommonFrame();
	}
}
