#ifndef UTILS_CONVERTIONS_INTLIB_H_
#define UTILS_CONVERTIONS_INTLIB_H_

#include "stm32f411xe.h"

int Int32ToByteArr(int32_t *source, int byteLength, uint8_t *destination);
int ByteArrToInt32(uint8_t *source, int byteLength, int32_t *destination);

#endif /* UTILS_CONVERTIONS_INTLIB_H_ */
