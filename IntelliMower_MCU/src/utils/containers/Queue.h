/*
 * Queue.h
 * A pretty fast FIFO queue
 * NOTE! This is not a circular queue
 */


#ifndef QUEUE_H_
#define QUEUE_H_


#include <stdint.h>


/*
 * Type container for the ques create an instance of queSet_t
*/
typedef struct {
	uint32_t bufSize;			// que index size
	uint32_t index;
	uint32_t outdex;
	int *buf;
} queue_t;


/*
* And use the following functions to use the fifo.
* Remember to use init to initialize the que size and use the
* FreeFifo to free up the memory when you're done using the fifo.
* NOTE! if new pushes exceeds que size they will be discarded
*/
void InitQueue( queue_t *que, uint32_t bufSize );
int  QueuePush( queue_t *que, int inData );
int  QueuePop( queue_t *que, int *outData );
void FreeQueue( queue_t *que );



#endif /* QUEUE_H_ */
