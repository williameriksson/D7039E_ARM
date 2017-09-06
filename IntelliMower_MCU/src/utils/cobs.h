#ifndef UTILS_COBS_H_
#define UTILS_COBS_H_

#include <stdint.h>
#include <string.h>

void stuffData(uint8_t*, int, uint8_t*);
void unStuffData(uint8_t*, int, uint8_t*);

#endif /* UTILS_COBS_H_ */
