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

void InitSteering() {
	InitMotorControl(&leftMotorController);
	InitMotorControl(&rightMotorController);
	SetMotorSpeed(&leftMotorController, 0);
	SetMotorSpeed(&rightMotorController, 0);
}

//Set motors to equal speed to drive straight forward.
void DriveForward(int speed) {
	SetMotorSpeed(&leftMotorController, speed);
	SetMotorSpeed(&rightMotorController, speed);
}

// Adjusts motor speed by adding Adjust(left/right) to motors current speed.
void AdjustSpeed(int leftAdjust, int rightAdjust) {
	int leftSpeed = GetMotorSpeed(&leftMotorController);
	int rightSpeed = GetMotorSpeed(&rightMotorController);
	SetMotorSpeed(&leftMotorController, leftSpeed+leftAdjust);
	SetMotorSpeed(&rightMotorController, rightSpeed+rightAdjust);
}

// Rotates left around it's own axis at speed (percentage)
void RotateLeft(int speed) {
	SetMotorSpeed(&leftMotorController, (0-speed));
	SetMotorSpeed(&rightMotorController, speed);
}

// Rotates right around it's own axis at speed (percentage)
void RotateRight(int speed) {
	SetMotorSpeed(&leftMotorController, speed);
	SetMotorSpeed(&rightMotorController, (0-speed));
}
