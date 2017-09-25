
#ifndef UTILS_FLOATLIB_H_
#define UTILS_FLOATLIB_H_

#include <stdint.h>

typedef union {
	float f;
	uint8_t byte[4];
} floatByte;

void FloatToByteArray(float*, int, uint8_t*);
void ByteArrayToFloat(uint8_t*, int, float*);

#endif /* UTILS_FLOATLIB_H_ */
