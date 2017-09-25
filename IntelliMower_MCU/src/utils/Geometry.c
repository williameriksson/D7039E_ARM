#include "Geometry.h"

void InitLineTwoPoints(Line *l, Point *pointA, Point *pointB) {
	l->k = (pointB->y - pointA->y) / (pointB->x - pointA->x);
	l->m = pointA->y - l->k * pointA->x;
	l->start = pointA;
	l->end = pointB;
}

double GetDistancePointToLine(Line *l, Point *point) {
	// Determine on which side of the line the point is
	double sign = (point->x - l->start->x) * (l->end->y - l->start->y) - (point->y - l->start->y) * (l->end->x - l->start->x);

	if (sign == 0.0) {
		return sign;
	}

	// Calculate the orthogonal distance between the line and the point
	double d = fabs((l->end->x - l->start->x) * (l->start->y - point->y) * (l->end->y - l->start->y)) / sqrt( pow((l->end->x - l->start->x), 2) + pow((l->end->y - l->start->y), 2) );

	if (sign < 0.0) {
		return -d;
	}
	return d;
}
