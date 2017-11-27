#include "MotorControl.h"

/*
 * Initializes a motorcontroller
 */
void InitMotorControl(MotorController *motorController) {
	InitTimerPWM(motorController->timer,
		motorController->channel,
		motorController->gpio,
		motorController->pin,
		motorController->pulseWidth,
		motorController->frequency
	);
}

void SetMotorSpeed(MotorController *motorController, int percentage) {
	motorController->currentSpeed = percentage;
	int neutralPWM = (motorController->pwmMax + motorController->pwmMin) / 2; //"neutral" PWM
	int varianceSpan =  neutralPWM - motorController->pwmMin; //max variance of PWM from "neutral"
	int newPWM = neutralPWM + ((varianceSpan * percentage)/10000);
	motorController->pulseWidth = newPWM;

	if(newPWM < motorController->pwmMin) { TimerSetPWM(motorController->timer, motorController->channel, motorController->pwmMin, motorController->frequency); }

	else if(newPWM > motorController->pwmMax) { TimerSetPWM(motorController->timer, motorController->channel, motorController->pwmMax, motorController->frequency); }

	else TimerSetPWM(motorController->timer, motorController->channel, newPWM, motorController->frequency); //sets the new PWM
	}

//returns what speed the motors are supposed to be running at(based on PWM)
//DONT USE THIS FUNCTION!
int GetMotorSpeed(MotorController *motorController) {
	int pulsewidth = motorController->pulseWidth;
	int neutralPWM = (motorController->pwmMax + motorController->pwmMin) / 2; //"neutral" PWM
	int varianceSpan =  neutralPWM - motorController->pwmMin; //max variance of PWM from "neutral"
	int currentDeviation = neutralPWM - pulsewidth;
	return currentDeviation*100 / varianceSpan;
}

void IncreaseMotorSpeed() {

}

void DecreaseMotorSpeed() {

}
