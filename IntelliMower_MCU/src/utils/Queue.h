/*
 * Queue.h
 * A pretty fast FIFO queue
 */


#ifndef QUEUE_H_
#define QUEUE_H_


#include <stdint.h>

//uint32_t queue[bufSize];	// queue container


typedef struct {
	uint32_t bufSize;			// que index size
	uint32_t bufIndex;
	uint32_t bufOutdex;
	int *buf;
} queSet_t;

void InitFifo( queSet_t *que, uint32_t bufSize );

int FifoPush( queSet_t *que, uint32_t inData );

int FifoPop( queSet_t *que, uint32_t *outData );


#endif /* QUEUE_H_ */
