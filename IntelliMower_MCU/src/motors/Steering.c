#include "Steering.h"

void InitSteering() {
	//initializes a motorController with immutable parameters.
	MotorController leftMotorController = {
		.timer = TIM2,
		.channel = 3,
		.gpio = GPIOA,
		.pin = 2,
		.pwmMin = 1000,
		.pwmMax = 2000,
		.frequency = 50,
	};

	MotorController rightMotorController = {
		.timer = TIM2,
		.channel = 4,
		.gpio = GPIOA,
		.pin = 3,
		.pwmMin = 1000,
		.pwmMax = 2000,
		.frequency = 50,
	};

	InitMotorControl(&leftMotorController);
	InitMotorControl(&rightMotorController);
	SetMotorSpeed(&leftMotorController, 0);
	SetMotorSpeed(&rightMotorController, 0);
}
