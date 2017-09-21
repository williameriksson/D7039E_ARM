/*
 * CircularBuffer.c
 *
 *  Created on: 15 sep. 2017
 *      Author: Sam
 */


#include "CircularBuffer.h"
#include <stdlib.h>
#include <stdint.h>


/*
 *
 */
void InitCircularBuffer( circularBuffer_t *que, uint32_t setSize ) {
	que->buf = ( int * )malloc( (setSize) * sizeof( int ) );
	que->index = 0;
	que->outdex = 0;
	que->overflow = 0;
	que->bufSize = setSize;
}

/*
 *  Push a uint32_t from in the FIFO
 */
int CircularBufferPush( circularBuffer_t *que, int inData ) {
	if( que->overflow && que->index == que->outdex )		// check if buf has overflowed, iterate outdex
		que->outdex = (que->outdex + 1) % que->bufSize;

	if( !( (que->index + 1) % que->bufSize ) ) 				// que overflowing on next push/pop
		que->overflow = 1;

	que->buf[que->index] = inData;
	que->index = (que->index + 1) % que->bufSize;

	return 1;
}

/*
 * Pop a uint32_t from the FIFO
 */
int CircularBufferPop( circularBuffer_t *que, int *outData ) {

	if ( !que->overflow && (que->index == que->outdex) )	// If buffer is empty and overflow false return
		return 0;

	*outData = que->buf[que->outdex];
	que->outdex = (que->outdex + 1) % ( que->bufSize );

	if( !que->outdex )
		que->overflow = 0;

	return 1;
}

/*
 *  Use this when you're done with the fifo
 *  othervize you're going to have a bad time.
 */
void FreeCircularBuffer( circularBuffer_t *que ) {
	free( que->buf );
	que->buf = 0;
	que->index = que->bufSize = que->outdex = que->overflow = 0;
}

// usage test case
/*
 *
InitCircularBuffer( &cBuf,4 );

CircularBufferPop( &cBuf, &queVar );	// does nothing

CircularBufferPush( &cBuf, 2 );
CircularBufferPush( &cBuf, 3 );
CircularBufferPush( &cBuf, 4 );
CircularBufferPush( &cBuf, 5 );

// buf size reached que will overflow
CircularBufferPush( &cBuf, 25 );		// will be inserted at index 0 que Overflow!
CircularBufferPush( &cBuf, 36 );		// will be inserted at index 1

queVar = cBuf.buf[0];		// 25
queVar = cBuf.buf[1];		// 36
queVar = cBuf.buf[2];		// 4
queVar = cBuf.buf[3];		// 5

CircularBufferPop( &cBuf, &queVar );	// 4
CircularBufferPop( &cBuf, &queVar ); 	// 5
CircularBufferPop( &cBuf, &queVar ); 	// 25
CircularBufferPop( &cBuf, &queVar ); 	// 36
CircularBufferPop( &cBuf, &queVar );	// does nothing
*/
