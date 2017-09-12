#include "Steering.h"

MotorController leftMotorController = {
		.timer = TIM2,
		.channel = 1,
		.gpio = GPIOA,
		.pin = 5,
		.pwmMin = 1000,
		.pwmMax = 2000,
		.frequency = 50,
		.pulseWidth = 1500,
	};

MotorController rightMotorController = {
		.timer = TIM2,
		.channel = 3,
		.gpio = GPIOB,
		.pin = 10,
		.pwmMin = 1000,
		.pwmMax = 2000,
		.frequency = 50,
		.pulseWidth = 1500,
	};

void InitSteering(Steering *steering) {
	//initializes a motorController with immutable parameters.

	steering->leftMotor = &leftMotorController;
	steering->rightMotor = &rightMotorController;

	InitMotorControl(&leftMotorController);
	InitMotorControl(&rightMotorController);
	SetMotorSpeed(&leftMotorController, 0);
	SetMotorSpeed(&rightMotorController, 0);
}

//Set motors to equal speed to drive straight forward.
void DriveForward(Steering *steering, int speed) {
	SetMotorSpeed(steering->leftMotor, speed);
	SetMotorSpeed(steering->rightMotor, speed);
}

// Adjusts motor speed by adding Adjust(left/right) to motors current speed.
void AdjustSpeed(Steering *steering, int leftAdjust, int rightAdjust) {
	int leftSpeed = GetMotorSpeed(steering->leftMotor);
	int rightSpeed = GetMotorSpeed(steering->leftMotor);
	SetMotorSpeed(steering->leftMotor, leftSpeed+leftAdjust);
	SetMotorSpeed(steering->leftMotor, leftSpeed+leftAdjust);
}

// Rotates left around it's own axis at speed (percentage)
void RotateLeft(Steering *steering, int speed) {
	SetMotorSpeed(steering->leftMotor, (0-speed));
	SetMotorSpeed(steering->rightMotor, speed);
}

// Rotates right around it's own axis at speed (percentage)
void RotateRight(Steering *steering, int speed) {
	SetMotorSpeed(steering->leftMotor, speed);
	SetMotorSpeed(steering->rightMotor, (0-speed));
}
