#include <stdio.h>
#include <math.h>
#include "Angle.h"

// returns the distance between two points in a cartesian coordinate system
double GetDistTwoPoints( Point p1, Point p2 ) {
   return sqrt( ( (p2.x - p1.x) * (p2.x - p1.x) ) + ( (p2.y - p1.y) * (p2.y - p1.y) ) );
}

// returns the smallest euler degree angle between three points
double GetTurnAngle( Point prvPoint, Point curPoint, Point nxtPoint ) {
  Point p1 = prvPoint;
  Point p2 = curPoint;
  Point p3 = nxtPoint;

  // distances between points
  double dP12 = GetDistTwoPoints( p1, p2 );
  double dP23 = GetDistTwoPoints( p2, p3 );
  double dP13 = GetDistTwoPoints( p1, p3 );

  double deg = 0;
  deg = acos( ( (dP12 * dP12) + (dP23 * dP23) - (dP13 * dP13) ) / ( 2 * dP12 * dP23 ) );
  
	/*double num = p2.y*(p1.x-p3.x) + p1.y*(p3.x-p2.x) + p3.y*(p2.x-p1.x);
	double den = (p2.x-p1.x)*(p1.x-p3.x) + (p2.y-p1.y)*(p1.y-p3.y);
	double ratio = num/den;

	double angleRad = atan(ratio);*/

  deg = (deg*180) / 3.14159265359;
  deg = fabs(deg);
  
  /*if ( deg > 180 ) {
    deg -= 180;
  }*/
  
  double turnDeg = 180 - deg;
  //float turnDeg = deg;
  
  if ( 0 > (nxtPoint.x - prvPoint.x) ) {
      turnDeg = -1*turnDeg;
  }else if ( nxtPoint.x == prvPoint.x ) {
	  if ( curPoint.y > nxtPoint.y ) {
		  turnDeg = 180;
	  }else{
		  turnDeg = 0;
		}
  }
  return turnDeg;
}
//	Point p0;
//	Point p1;
//	Point p2;
//	Point p3;
//
//	float dP12 = 0;
//	float dP23 = 0;
//	float dP13 = 0;
//
//int main(){
//
//	p1.x = 70.25;
//	p1.y = 34.25;
//
//	p2.x = 80.25;
//	p2.y = 41.25;
//
//	p3.x = 40.25;
//	p3.y = 76.34;
//
//	dP12 = GetDistTwoPoints( p2, p1 );
//	dP23 = GetDistTwoPoints( p2, p3 );
//	dP13 = GetDistTwoPoints( p1, p3 );
//
//    printf("dP12: %f\n", dP12);
//	printf("dP23: %f\n", dP23);
//	printf("dP13: %f\n", dP13);
//
//	float deg = GetTurnAngle( p1, p2, p3 );
//	printf("turn angle: %f\n", deg);
//
//
//    return 0;
//}
