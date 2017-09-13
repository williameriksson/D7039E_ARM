/*
 * Queue.h
 * A pretty fast FIFO queue
 */


#ifndef QUEUE_H_
#define QUEUE_H_


#include <stdint.h>



/*
 * type container for the FIFO create an instance of queSet_t
 * and use the following functions to use the fifo.
 * Remember to use init to initialize the que size and use the
 * FreeFifo to free up the memory when you're done using the fifo.
*/
typedef struct {
	uint32_t bufSize;			// que index size
	uint32_t bufIndex;
	uint32_t bufOutdex;
	int *buf;
} queSet_t;


void InitFifo( queSet_t *que, uint32_t bufSize );
int FifoPush( queSet_t *que, uint32_t inData );
int FifoPop( queSet_t *que, uint32_t *outData );
void FreeFifo( queSet_t *que );

#endif /* QUEUE_H_ */
