#include "intLib.h"

int Int32ToByteArr(int32_t *source, int byteLength, uint8_t *destination) {
	for(int i = 0; i < byteLength; i++) {
		int32_t local = source[i/4];
		destination[i] |= (local >> (24-(8*(i%4))));
	}
	return 0;
}

int ByteArrToInt32(uint8_t *source, int byteLength, int32_t *destination) {
	for(int i = 0; i < byteLength; i++) {
		destination[i/4] |= (source[i] << (24-(8*(i%4))));
	}
	return 0;
}


