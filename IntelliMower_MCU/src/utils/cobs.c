/*
* Contains code for the COBS (Consistent Overhead Byte Stuffing) encoding and decoding
 */
#include "Cobs.h"

//Encodes data with COBS, dst MUST be length + 2 at least.
void stuffData(uint8_t *ptr, int length_ptr, uint8_t *dst) {
	int index = length_ptr - 1; //starts at the end of indata string
	dst[length_ptr + 1] = 0x00; //marks end of packet.
	int previousZero = length_ptr + 1; //marks zero already encountered (starts at end-of-string zero)
	while (index >= 0) {
		if (ptr[index] == 0x00) {
			dst[index + 1] = previousZero - index - 1;
			previousZero = index + 1;
		}
		else {
			dst[index + 1] = ptr[index];
		}
		index--;
	}
	dst[index + 1] = previousZero;
}

//Decodes the COBS data and puts it into dst.
void unStuffData(uint8_t *ptr, int length_ptr, uint8_t *dst) {
	int index = 1;
	int nextZero = ptr[0];
	while (index < length_ptr) {
		if (index == nextZero && ptr[index] != 0x00) {
			dst[index - 1] = 0; //encoded zero reached
			nextZero += ptr[index];
		}
		else if (index == nextZero && ptr[index] == 0x00) {
			break; //end of packet reached
		}
		else {
			dst[index - 1] = ptr[index]; //is not encoded zero or end of packet.
		}
		index++;
	}
}
