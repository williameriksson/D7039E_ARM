/*
 * Queue.h
 * A pretty fast FIFO queue
 */



#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>

int FifoPush( const uint8_t data );

int FifoPop( uint8_t *data );


#endif /* QUEUE_H_ */
