#include "floatLib.h"

void FloatToByteArray(float *src, int size_src, uint8_t *dst) {
	int i = 0;
	while(i < size_src) {
		floatByte fb;
		fb.f = src[i];
		for(int j = 0; j < 4; j++) {
			dst[(4*i)+j] = fb.byte[j];
		}
		i++;
	}
}

void ByteArrayToFloat(uint8_t *src, int size_src, float *dst) {
	int i = 0;
	while(i < size_src/4) {
		floatByte fb;
		for(int j = 0; j < 4; j++) {
			fb.byte[j] = src[(4*i)+j];
		}
		dst[i] = fb.f;
		i++;
	}
}
