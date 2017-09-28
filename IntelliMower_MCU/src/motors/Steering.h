#ifndef MOTORS_STEERING_H_
#define MOTORS_STEERING_H_

#include "stm32f411xe.h"
#include "MotorControl.h"

//typedef struct {
//	MotorController *leftMotor;
//	MotorController *rightMotor;
//}Steering;

void InitSteering();
void RotateLeft(int speed);
void RotateRight(int speed);
void DriveForward(int speed);
void AdjustSpeed(int leftAdjust, int rightAdjust);

#endif /* MOTORS_STEERING_H_ */
