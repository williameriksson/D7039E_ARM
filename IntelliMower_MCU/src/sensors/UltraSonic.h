#ifndef SENSORS_ULTRASONIC_H_
#define SENSORS_ULTRASONIC_H_

#include "utils/Timer.h"
#include "stm32f4xx_nucleo.h"

typedef struct {
	int distances[4];
}DistanceSensors;

DistanceSensors distanceSensors;

void InitUltrasonic(void);

#endif /* SENSORS_ULTRASONIC_H_ */
