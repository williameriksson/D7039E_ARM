#include "geometry.h"

void initLineTwoPoints(line *l, double x1, double y1, double x2, double y2) {
	l->k = (y2 - y1) / (x2 - x1);
	l->m = y1 - l->k * x1;
	l->x1 = x1;
	l->y1 = y1;
	l->x2 = x2;
	l->y2 = y2;
}

double getDistanceFromLine(line *l, double x, double y) {
	// Determine on which side of the line the point is
	double sign = (x - l->x1) * (l->y2 - l->y1) - (y - l->y1) * (l->x2 - l->x1);

	if (sign == 0.0) {
		return sign;
	}

	// Calculate the orthogonal distance between the line and the point
	double d = fabs((l->x2 - l->x1) * (l->y1 - y) * (l->y2 - l->y1)) / sqrt( pow((l->x2 - l->x1), 2) + pow((l->y2 - l->y1), 2) );

	if (sign < 0.0) {
		return -d;
	}
	return d;
}
