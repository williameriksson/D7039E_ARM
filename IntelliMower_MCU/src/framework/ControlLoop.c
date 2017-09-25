/*
 *  Responsible for fine tuning the path or course with a pid controller
 */

#include <framework/CmdSystem.h>
#include "ControlLoop.h"

#include "motors/Steering.h"
#include "utils/PIDcontroller.h"
#include "utils/Timer.h"

PID lineFollowController;

Point startPos;
Point targetPos;
Point currentPos;
Line line;

double distFromLineTEST;

/*
 * Initializes the PID controller and sets the target coordinate.
 */
void InitControlLoop(Point *startPoint, Point *targetPoint) {
	startPos.x = startPoint->x;
	startPos.y = startPoint->y;
	currentPos.x = startPoint->x;
	currentPos.y = startPoint->y;
	targetPos.x = targetPoint->x;
	targetPos.y = targetPoint->y;

	InitLineTwoPoints(&line, &startPos, &targetPos);
	distFromLineTEST = GetDistancePointToLine(&line, &currentPos);

	int loopTime = 1000;
//	Init a PID controller with parameters, ref value should
//	be distance to the line it should hold (typically 0).
	InitController(&lineFollowController, 0, 1.0f, 0, 0, loopTime);

//	Set up an interrupt to that runs at x hertz, this is the control loop time.
	InitTimerInterrupt(TIM9, loopTime);

}

//ran by the interrupt handler
void LoopController() {
	//	At every interrupt:
	//	Check distance from line (use line eq's),
	//	feed this into the PID
	double distanceFromLine = GetDistancePointToLine(&line, &currentPos);
	distFromLineTEST = distanceFromLine;
	float output = RunController(&lineFollowController, distanceFromLine, 0);

	//	Use the return value from the PID to adjust steering accordingly.
	AdjustSpeed(output, -output);
}

void UpdatePIDValue(Point *newPos) {
	currentPos.x = newPos->x;
	currentPos.y = newPos->y;
	distFromLineTEST = GetDistancePointToLine(&line, &currentPos);

}

void StopController() {
	StopTimerInterrupt(TIM9);
}

//currently assigned by ControlLoop.c as internal interrupt for PID
void TIM1_BRK_TIM9_IRQHandler() {
	LoopController();
	TIM9->SR &= ~TIM_SR_TIF;
}

