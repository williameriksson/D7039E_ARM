/*
 * geometry.h
 *
 *  Created on: Sep 6, 2017
 *      Author: William
 */

#ifndef UTILS_GEOMETRY_H_
#define UTILS_GEOMETRY_H_

#include <math.h>

typedef struct {
	double k;
	double m;
	double x1;
	double y1;
	double x2;
	double y2;
} line;

void initLineTwoPoints(line*, double, double, double, double);
double getDistanceFromLine(line*, double, double);


#endif /* UTILS_GEOMETRY_H_ */
