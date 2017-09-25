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
	double x;
	double y;
} Point;

typedef struct {
	double k;
	double m;
	Point *start;
	Point *end;
} Line;


void InitLineTwoPoints(Line*, Point*, Point*);
double GetDistancePointToLine(Line*, Point*);
int Equals(Point*, Point*);


#endif /* UTILS_GEOMETRY_H_ */
