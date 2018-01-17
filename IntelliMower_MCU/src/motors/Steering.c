#include "Steering.h"

/*
 * Contains methods for controlling the steering of the Mower.
 * All speeds can be given in the range [-100, 100]
 * note: rotating left at 100 is the same as rotating right -100.
 */

MotorController leftMotorController = {
		.timer = TIM2,
		.channel = 1,
		.gpio = GPIOA,
		.pin = 5,
		.pwmMin = 1,
		.pwmMax = 49,
		.frequency = 20000,
		.pulseWidth = 25,
		.currentSpeed = 0,
	};

MotorController rightMotorController = {
		.timer = TIM2,
		.channel = 3,
		.gpio = GPIOB,
		.pin = 10,
		.pwmMin = 1,
		.pwmMax = 49,
		.frequency = 20000,
		.pulseWidth = 25,
		.currentSpeed = 0,
	};

/* Turning procedure vars */
float initialHeading;
float degreesToTurn;

/* Driving procedure vars */
Point prevPos = {.x = 0.0, .y = 0.0};
Point targetPos = {.x = 0.0, .y = 0.0};
double prevDistance = 1000000.0;
double acceptedTargetDeviation = 5.0;
int driveSpeed = 0;

/* STM studio monitoring */
int leftSpeedGlobal;
int rightSpeedGlobal;
int leftPwGlobal;
int rightPwGlobal;

void InitSteering() {
	GpioEnable(GPIOA);
	GpioSetOutput(GPIOA, 0);
	GpioSetPinLow(GPIOA, 0);
	InitMotorControl(&leftMotorController);
	InitMotorControl(&rightMotorController);
	SetMotorSpeed(&leftMotorController, 0);
	SetMotorSpeed(&rightMotorController, 0);
	prevPosAngle = 0.0;
	_state = IDLE;
}

//Set motors to equal speed to drive straight forward.
int DriveForward(int speed) {
	if(speed > 10000 || speed < -10000) {
		int newSpeed = 10000*(speed/abs(speed));
		SetMotorSpeed(&leftMotorController, newSpeed);
		SetMotorSpeed(&rightMotorController, newSpeed);
		return 1; //trying to set speed higher than allowed.
	}
	SetMotorSpeed(&leftMotorController, speed);
	SetMotorSpeed(&rightMotorController, speed);
	return 0; //all fine
}

// Adjusts motor speed by adding Adjust(left/right) to motors current speed.
int AdjustSpeed(int leftAdjust, int rightAdjust) {
//	int leftSpeed = GetMotorSpeed(&leftMotorController);
//	int rightSpeed = GetMotorSpeed(&rightMotorController);
	int leftSpeed = leftMotorController.currentSpeed;
	int rightSpeed = rightMotorController.currentSpeed;
	int newLeftSpeed = leftSpeed+leftAdjust;
	int newRightSpeed = rightSpeed+rightAdjust;

	leftSpeedGlobal = leftSpeed;
	rightSpeedGlobal = rightSpeed;
	leftPwGlobal = TIM2->CCR1;
	rightPwGlobal = TIM2->CCR3;

	if(abs(newLeftSpeed) > 10000 && abs(newRightSpeed) > 10000) {
		SetMotorSpeed(&leftMotorController, 10000*(newLeftSpeed/abs(newLeftSpeed)));
		SetMotorSpeed(&rightMotorController, 10000*(newRightSpeed/abs(newRightSpeed)));
		return 1; //boths speeds are maxed, slow the F down!
	}
	if(abs(newLeftSpeed) > 10000) {
		SetMotorSpeed(&leftMotorController, 10000*(newLeftSpeed/abs(newLeftSpeed)));
		SetMotorSpeed(&rightMotorController, newRightSpeed);
		return 1; //the left speed is already maxed
	}
	if(abs(newRightSpeed) > 10000) {
		SetMotorSpeed(&rightMotorController, 10000*(newRightSpeed/abs(newRightSpeed)));
		SetMotorSpeed(&leftMotorController, newLeftSpeed);
		return 1; //the right speed is already maxed
	}

	SetMotorSpeed(&leftMotorController, newLeftSpeed);
	SetMotorSpeed(&rightMotorController, newRightSpeed);
	return 0; //all fine
}

int GetLeftSpeed() {
	return leftMotorController.currentSpeed;
}

int GetRightSpeed() {
	return rightMotorController.currentSpeed;
}

// Rotates left around it's own axis at speed (percentage)
int RotateLeft(int speed) {
	if(speed > 10000 || speed < -10000) {
		SetMotorSpeed(&leftMotorController, 10000*(-speed/abs(speed)));
		SetMotorSpeed(&rightMotorController, 10000*(speed/abs(speed)));
		return 1; //trying to set speed higher than allowed.
	}
	SetMotorSpeed(&leftMotorController, (0-speed));
	SetMotorSpeed(&rightMotorController, speed);
	return 0; //all fine
}

// Rotates right around it's own axis at speed (percentage)
int RotateRight(int speed) {
	if(speed > 10000 || speed < -10000) {
		SetMotorSpeed(&leftMotorController, 10000*(speed/abs(speed)));
		SetMotorSpeed(&rightMotorController, 10000*(-speed/abs(speed)));
		return 1; //trying to set speed higher than allowed.
	}
	SetMotorSpeed(&leftMotorController, speed);
	SetMotorSpeed(&rightMotorController, (0-speed));
	return 0; //all fine
}

/* Startes rotating procedure, this should be checked in Steeringhandler for termination*/
int RotateDegrees(float degToTurn, float currentHeading) {
	if(_state == IDLE) {
		_state = TURNING;
		initialHeading = currentHeading;
		degreesToTurn = degToTurn;
		if (degToTurn < 0) {
			RotateLeft(3000);
		}
		else {
			RotateRight(3000);
		}
		return 0; //all fine
	}
	return 1; //trying to turn when not in IDLE state.
}

void SteeringHandler(float currentHeading) {
	if(_state == TURNING) {
		//is in the process of turning x degrees.
		if(abs(currentHeading) >= abs(initialHeading + degreesToTurn) ) { //turn has been reached.
			//has turned desired degrees.
			DriveForward(-3000);
			prevPosAngle = posAngle;
			_state = DRIVING;
		}
	}
}

void DrivingHandler(Point *pos) {
	if(_state == DRIVING) {
		double distance = GetDistTwoPoints(*pos, targetPos);
		if(distance <= prevDistance || distance <= acceptedTargetDeviation) {
		//The distance to target is decreasing or within accepted range
			prevDistance = distance;
		}
		else {
			//It has reached target or can't get closer.
			DriveForward(0);
			prevDistance = 1000000;
			_state = IDLE;
//			SendIdle(); //informs the r.pi that it's idle again.
//			while(_state == IDLE) {
//				SendIdle(); //informs the r.pi that it's idle again.
//				for(int i = 0; i < 10000000; i++) {
//
//				}
//			}
		}
	}
	else {
		prevDistance = 1000000;
	}
}

void CheckIdle() {
	if(_state == IDLE) {
	}
}

int SetDriveTarget(Point newTarget) {
	targetPos.x = newTarget.x;
	targetPos.y = newTarget.y;
	return 0;
}

int SetDriveSpeed(int speed) {
	driveSpeed = speed;
	return 0;
}

