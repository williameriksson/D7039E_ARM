/*
 *  Responsible for fine tuning the path or course with a pid controller
 */

#include <framework/CmdSystem.h>
#include "ControlLoop.h"

#include "motors/Steering.h"
#include "utils/PIDcontroller.h"



void InitControlLoop() {
//	Init a PID controller with parameters, ref value should
//	be distance to the line it should hold (typically 0).

//	Set up an interrupt to that runs at x hertz, this is the control loop time.
//	At every interrupt:
//	Check distance from line (use line eq's),
//	feed this into the PID

//	Use the return value from the PID to adjust steering accordingly.
}

