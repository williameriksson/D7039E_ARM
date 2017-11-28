#ifndef SENSORS_ULTRASONIC_H_
#define SENSORS_ULTRASONIC_H_

#include "utils/Timer.h"
#include "stm32f4xx_nucleo.h"

typedef struct {
	double distanceOuterLeft;
	double distanceInnerLeft;
	double distanceOuterRight;
	double distanceInnerRight;
}DistanceSensors;

DistanceSensors distanceSensors;

void InitUltrasonic(void);

#endif /* SENSORS_ULTRASONIC_H_ */
