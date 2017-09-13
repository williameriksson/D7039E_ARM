
#include "Queue.h"

#include <stdlib.h>

// private variables
void InitFifo( queSet_t *que, uint32_t setSize ) {
	que->buf = ( int * )malloc( setSize * sizeof( int ) );
	que->bufIndex = 0;
	que->bufSize = setSize;
}

/*
 *  Push a uint32_t from in the FIFO
 */
int FifoPush( queSet_t *que, uint32_t inData ) {
	if ( que->bufIndex == ( ( que->bufOutdex - 1 + que->bufSize ) % que->bufSize ) )
		return 0;
	que->buf[que->bufIndex] = inData;
	que->bufIndex = (que->bufIndex + 1) % que->bufSize;
	return 1;
}

/*
 * Pop a uint32_t from the FIFO
 */
int FifoPop( queSet_t *que, uint32_t *outData ) {
	if ( que->bufIndex == que->bufOutdex )
		return 0;
	*outData = que->buf[que->bufOutdex];
	que->bufOutdex = (que->bufOutdex + 1) % que->bufSize;
	return 1;
}

/*
 * insert this for a + element in que for example if the
 * que size is over buf size this code can resiz the que.
 *
 */
/*
void insertArray(Array *a, int element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size
  if (a->used == a->size) {
    a->size *= 2;
    a->array = (int *)realloc(a->array, a->size * sizeof(int));
  }
  a->array[a->used++] = element;
}
*/

/*
 *  Use this when you're done with the fifo
 *  othervize you're going to have a bad time. ( memory leakages! )
 */
void ShutdownFifo( queSet_t *que ) {
	free( que->buf );
	que->buf = 0;
	que->bufIndex = que->bufSize = 0;
}
