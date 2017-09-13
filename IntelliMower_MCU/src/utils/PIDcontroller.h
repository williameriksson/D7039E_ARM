#ifndef CONTROLLER_PIDCONTROLLER_H_
#define CONTROLLER_PIDCONTROLLER_H_

typedef struct {
	float referencePoint; //reference point to control around
	float Kp, Ki, Kd; //PID parameters
	float integralError; //the accumulated integral error
	float previousError; //the calculated error in the previous loop
	int looptime; //time (in ms) between controller runs
}PID;

void InitController(PID *controller, float ref, float Kp, float Ki, float Kd, int looptime);
float RunController(PID*, float, int);
void ChangeReference(PID*, float);
void ChangeParameters(PID*, float, float, float);
void ResetPIDError(PID*);

#endif /* CONTROLLER_PIDCONTROLLER_H_ */
