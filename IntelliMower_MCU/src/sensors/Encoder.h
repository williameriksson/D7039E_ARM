#ifndef SENSORS_ENCODER_H_
#define SENSORS_ENCODER_H_

#include "utils/Timer.h"
#include "stm32f4xx_nucleo.h"
#include "motors/Steering.h"
#include <math.h>

typedef struct {
	int encoderSpeed;
	float encoderDistance; //in centimeters
	int reg, prevReg;
	char timeout;
}RotaryEncoder;

RotaryEncoder leftEncoder;
RotaryEncoder rightEncoder;

//double xxPos;
//double yyPos;
double posAngle;

Point currentPosition;

double GetPosAngle();
void InitEncoders();
int GetEncoderSpeed(int reg, int prevReg, int direction);
void ResetEncoder(RotaryEncoder *encoder);

#endif /* SENSORS_ENCODER_H_ */
