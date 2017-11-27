#include "Steering.h"

/*
 * Contains methods for controlling the steering of the Mower.
 * All speeds can be given in the range [-100, 100]
 * note: rotating left at 100 is the same as rotating right -100.
 */

MotorController leftMotorController = {
		.timer = TIM2,
		.channel = 1, //change to be ch 1
		.gpio = GPIOA, //change to be GPIOA
		.pin = 5, //change to pin 5
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

STEERING_STATE steering_state;
float initialHeading;
float degreesToTurn;

int leftSpeedGlobal;
int rightSpeedGlobal;
int leftPwGlobal;
int rightPwGlobal;

void InitSteering() {
	InitMotorControl(&leftMotorController);
	InitMotorControl(&rightMotorController);
	SetMotorSpeed(&leftMotorController, 0);
	SetMotorSpeed(&rightMotorController, 0);
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

int RotateDegrees(float degToTurn, float currentHeading) {
	if(steering_state = IDLE) {
	steering_state = TURNING;
	initialHeading = currentHeading;
	degreesToTurn = degToTurn;
	return 0;
	}
	return 1; //trying to turn when not in IDLE state.
}


void SteeringHandler(float currentHeading) {
	if(steering_state == TURNING) {
		//is in the process of turning x degrees.
		if(abs(currentHeading) >= abs(initialHeading + degreesToTurn) ) { //turn has been reached.
			//has turned desired degrees.
			steering_state = IDLE;
		}
	}
}
