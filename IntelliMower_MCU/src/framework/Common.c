/*
 * Common.c
 */

#include "Common.h"
#include "CmdSystem.h"
#include "KeyInput.h"
#include "ControlLoop.h"

static bool isInitialized = false;

/*
 *
 */
void CommonInit() {

	isInitialized = True;
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
	 // After power on the MCU goes in idle state and waits for cmds from RPI

	 if ( isIdle ){
		 if ( sensor ) {	// Small obstacle detected
		 	// Do Something Based on sensor
			// Alert Raspberry and go into idle if the obstacle is too large
		 {

	 } else if ( isCutting ) {	// In cutting mode run control loop

		 if ( sensor ) {	// Small obstacle detected
		 	// Do Something Based on sensor
			// Alert Raspberry and go into idle if the obstacle is too large
		 {

		oldCmd = newCmd;
		newCmd = PumpCmds();

		case : event {


		}

		 }
	 }
*/

}

