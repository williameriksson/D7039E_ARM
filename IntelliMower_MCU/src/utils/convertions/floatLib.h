
#ifndef UTILS_FLOATLIB_H_
#define UTILS_FLOATLIB_H_

#include <stdint.h>

typedef union {
	float f;
	uint8_t byte[4];
} floatByte;

typedef union {
	double d;
	uint8_t byte[8];
} doubleByte;

void FloatToByteArray(float*, int, uint8_t*);
void ByteArrayToFloat(uint8_t*, int, float*);
void DoubleToByteArray(double *src, int size_src, uint8_t *dst);
void ByteArrayToDouble(uint8_t *src, int size_src, double *dst);

#endif /* UTILS_FLOATLIB_H_ */
