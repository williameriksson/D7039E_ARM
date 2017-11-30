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

void DoubleToByteArray(double *src, int size_src, uint8_t *dst) {
	int i = 0;
	while(i < size_src) {
		doubleByte db;
		db.d = src[i];
		for(int j = 0; j < 8; j++) {
			dst[(8*i)+j] = db.byte[j];
		}
		i++;
	}
}

void ByteArrayToDouble(uint8_t *src, int size_src, double *dst) {
	int i = 0;
	while(i < size_src/8) {
		doubleByte db;
		for(int j = 0; j < 8; j++) {
			db.byte[j] = src[(8*i)+j];
		}
		dst[i] = db.d;
		i++;
	}
}
