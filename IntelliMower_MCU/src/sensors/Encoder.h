#ifndef SENSORS_ENCODER_H_
#define SENSORS_ENCODER_H_

#include "utils/Timer.h"
#include "stm32f4xx_nucleo.h"

typedef struct {
	int encoderSpeed;
	int reg, prevReg;
	char timeout;
}RotaryEncoder;

void InitEncoders();
int GetEncoderSpeed(int reg, int prevReg, int direction);
void ResetEncoder(RotaryEncoder *encoder);

#endif /* SENSORS_ENCODER_H_ */
