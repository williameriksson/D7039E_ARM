/*
 *  Responsible for fine tuning the path or course with a pid controller
 */

#include <framework/CmdSystem.h>
#include "ControlLoop.h"
#include "motors/Steering.h"
#include "utils/PIDcontroller.h"
#include "utils/Timer.h"
#include "sensors/Gyroscope.h"
#include "sensors/Encoder.h"

PID lineFollowController;

Point startPos;
Point targetPos;
Point currentPos;
Line line;

/* For STM studio viewer  */
double distFromLineTEST;
double outputPID;

int allowedRange;
int minAdjust; //for capping controller
int maxAdjust;
int disSpeed;

/*
 * Initializes the PID controller and sets the target coordinate.
 */
void InitControlLoop(Point *startPoint, Point *targetPoint, int speed) {
	disSpeed = speed;
	allowedRange = 1000;
	minAdjust = disSpeed - allowedRange;
	maxAdjust = disSpeed + allowedRange;
	startPos.x = startPoint->x;
	startPos.y = startPoint->y;
	currentPos.x = startPoint->x;
	currentPos.y = startPoint->y;
	targetPos.x = targetPoint->x;
	targetPos.y = targetPoint->y;

	InitLineTwoPoints(&line, &startPos, &targetPos);
	distFromLineTEST = GetDistancePointToLine(&line, &currentPos);

	int loopTime = 10;
//	Init a PID controller with parameters, ref value should
//	be distance to the line it should hold (typically 0).
	InitController(&lineFollowController, 0, 0.5f, 0.0f, 0.1f, loopTime); //dist from line regulator settings
//	InitController(&lineFollowController, 0, 0.5f, 0.0, 1.0, loopTime);

//	Set up an interrupt to that runs at x hertz, this is the control loop time.
	InitTimerInterrupt(TIM10, loopTime, 20);
}
double prevAngleOffset;
double weightedAngle;
//ran by the interrupt handler
void LoopController() {
	//	At every interrupt:
	//	Check distance from line (use line eq's),
	//	feed this into the PID
	double distanceFromLine = GetDistancePointToLine(&line, &currentPos);
	distFromLineTEST = distanceFromLine; //for value viewer

//	double angleOffset = gyro.yaw; //Angle from gyro
	double angleOffset = GetPosAngle(); //Angle from encoders

	float output = RunController(&lineFollowController, distanceFromLine, 0); //regulate on dist from line.
//	float output = RunController(&lineFollowController, angleOffset, 0); //regulate on gyro

	outputPID = output; //for STM studio monitoring

	weightedAngle = weightedAngle - (angleOffset - prevAngleOffset);
	weightedAngle = weightedAngle * 0.99 + posAngle * 0.01;
	prevAngleOffset = angleOffset;
//	output += (weightedAngle * 0.25); //feedforward
//	output += (angleOffset * 0.25);

	//	Use the return value from the PID to adjust steering accordingly.
	int leftAdjust = (int)output;
	int rightAdjust = -1*(int)output;

	if((leftAdjust + GetLeftSpeed() < minAdjust) || (leftAdjust + GetLeftSpeed() > maxAdjust)) {
		AdjustSpeed(disSpeed + ((abs(leftAdjust)/leftAdjust) * allowedRange) - GetLeftSpeed(), 0);
		AdjustSpeed(0, disSpeed + ((abs(rightAdjust)/rightAdjust) * allowedRange) - GetRightSpeed());
	}
	else {
		AdjustSpeed(leftAdjust, rightAdjust);
	}
}

void UpdatePIDValue(Point *newPos) {
	currentPos.x = newPos->x;
	currentPos.y = newPos->y;
//	distFromLineTEST = GetDistancePointToLine(&line, &currentPos); //for stm studio viewer
}

void StopController() {
	StopTimerInterrupt(TIM10);
}

//currently assigned by ControlLoop.c as internal interrupt for PID
void TIM1_UP_TIM10_IRQHandler() {
	LoopController();
	TIM10->SR &= ~TIM_SR_UIF;
}

