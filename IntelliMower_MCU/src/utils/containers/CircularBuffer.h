/*
 * CircularBuffer.h
 *
 *	Notes!
 *  	Init: Dynamic initialization size.
 *  	Push: Is circular you can always push. If buffer overflows the buffer will overwrite the oldest value in the buffer.
 *  	Pop:  Will always pop the oldest value in the buffer ( fifo behavior )
 *
 */

#ifndef UTILS_CIRCULARBUFFER_H_
#define UTILS_CIRCULARBUFFER_H_

#include <stdint.h>

/*
 * Type container for the ques create an instance of queSet_t
*/
typedef struct {
	uint32_t bufSize;			// que index size
	uint32_t index;
	uint32_t outdex;
	uint32_t overflow;			// bool value
	int *buf;
} circularBuffer_t;


/*
* And use the following functions to use the fifo.
* Remember to use init to initialize the que size and use the
* FreeFifo to free up the memory when you're done using the fifo.
* NOTE! if new pushes exceeds que size they will be discarded
*/
void InitCircularBuffer( circularBuffer_t *cBuf, uint32_t bufSize );
int  CircularBufferPush( circularBuffer_t *cBuf, int inData );
int  CircularBufferPop( circularBuffer_t *cBuf, int *outData );
void FreeCircularBuffer( circularBuffer_t *cBuf );


#endif /* UTILS_CIRCULARBUFFER_H_ */
