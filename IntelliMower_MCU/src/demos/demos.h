#ifndef DEMOS_DEMOS_H_
#define DEMOS_DEMOS_H_

#include <math.h>
#include "stm32f411xe.h"
#include "utils/Geometry.h"
#include "sensors/Gyroscope.h"
#include "sensors/Encoder.h"
#include "motors/Steering.h"

int DemoGyroNav(int state, int speed);

#endif /* DEMOS_DEMOS_H_ */
