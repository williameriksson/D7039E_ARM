
#include "Queue.h"


// private variables


/*
 *  Push a uint32_t from in the FIFO
 */
/*int FifoPush( const queSet_t *queSet, uint32_t inData ) {
	if (queSet->bufIndex == ((queSet->bufOutdex - 1 + queSet->bufSize) % queSet->bufSize))
		return 0;
	queSet->queue[queSet->bufIndex] = inData;
	queSet->bufIndex = (queSet->bufIndex + 1) % queSet->bufSize;
	return 1;
}*/

/*
 * Pop a uint32_t from the FIFO
 */
/*int FifoPop( const queSet_t *queSet, uint32_t *outData ) {
	if (queSet->bufIndex == queSet->bufOutdex)
		return 0;
	*outData = queSet->queue[queSet->bufOutdex];
	queSet->bufOutdex = (queSet->bufOutdex + 1) % queSet->bufSize;
	return 1;
}*/

