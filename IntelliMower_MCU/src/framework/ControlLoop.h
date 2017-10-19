

#ifndef CONTROLLOOP_H_
#define CONTROLLOOP_H_

#include "utils/Geometry.h"

void InitControlLoop(Point *startPoint, Point *targetPoint, int speed);
void LoopController();
void UpdatePIDValue(Point *newPos);
void StopController();

#endif /* CONTROLLOOP_H_ */
