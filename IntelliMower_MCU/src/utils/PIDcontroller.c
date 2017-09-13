#include "PIDcontroller.h"

//initializes the controller with specified reference and PID params.
void InitController(PID *controller, float ref, float Kp, float Ki, float Kd, int looptime) {
	controller->referencePoint = ref;
	controller->Kp = Kp;
	controller->Ki = Ki;
	controller->Kd = Kd;
	controller->integralError = 0.0f;
	controller->previousError = 0.0f;
	controller->looptime = looptime;
}
/*
 * Calculates the control-effort required based on current value.
 */
float RunController(PID *controller, float currentValue, int isSaturated) {
	float error = (controller->referencePoint - currentValue);
	if(!isSaturated) {
		controller->integralError = controller->integralError + error;
	}
	controller->previousError = error;
	float derivative = controller->previousError - error;
	float integral = controller->integralError;
	float output = controller->Kp * error + (controller->Ki * integral * ((float)controller->looptime / 1000.0f)) + (controller->Kd * derivative / ((float)controller->looptime / 1000.0f));
	return output;
}

void ChangeReference(PID *controller, float newRef) {
	controller->referencePoint = newRef;
}

void ChangeParameters(PID *controller, float P, float I, float D) {
	controller->Kp = P;
	controller->Ki = I;
	controller->Kd = D;
	resetPIDError(controller);
}

void ResetPIDError(PID *controller) {
	controller->integralError = 0.0f;
	controller->previousError = 0.0f;
}

