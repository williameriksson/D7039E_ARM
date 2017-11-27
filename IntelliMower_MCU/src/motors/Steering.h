#ifndef MOTORS_STEERING_H_
#define MOTORS_STEERING_H_

#include "stm32f411xe.h"
#include "MotorControl.h"
#include "stdlib.h"

//typedef struct {
//	MotorController *leftMotor;
//	MotorController *rightMotor;
//}Steering;

typedef enum {
	IDLE,
	TURNING
}STEERING_STATE;

void InitSteering();
int RotateLeft(int speed);
int RotateRight(int speed);
int DriveForward(int speed);
int AdjustSpeed(int leftAdjust, int rightAdjust);
int GetLeftSpeed();
int GetRightSpeed();

#endif /* MOTORS_STEERING_H_ */
