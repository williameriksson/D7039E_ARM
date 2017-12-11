#ifndef MOTORS_STEERING_H_
#define MOTORS_STEERING_H_

#include "stm32f411xe.h"
#include "MotorControl.h"
#include "com/UART.h"
#include "stdlib.h"
#include "utils/Geometry.h"
#include "utils/Angle.h"
#include "sensors/Encoder.h"

//typedef struct {
//	MotorController *leftMotor;
//	MotorController *rightMotor;
//}Steering;

typedef enum {
	IDLE,
	TURNING,
	DRIVING
}_STATE;

_STATE _state;
double prevPosAngle;

void InitSteering();
int RotateLeft(int speed);
int RotateRight(int speed);
int DriveForward(int speed);
int AdjustSpeed(int leftAdjust, int rightAdjust);
int GetLeftSpeed();
int GetRightSpeed();
int RotateDegrees(float degToTurn, float currentHeading);
void SteeringHandler(float currentHeading);
void DrivingHandler(Point *curP);
int SetDriveTarget(Point newTarget);
int SetDriveSpeed(int speed);
void CheckIdle();

#endif /* MOTORS_STEERING_H_ */
