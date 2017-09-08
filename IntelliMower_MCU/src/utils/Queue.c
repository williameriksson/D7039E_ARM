
#include "Queue.h"


// private variables

#define MAX_BUF_SIZE 10		// que index size

static volatile uint8_t queue[MAX_BUF_SIZE];		// queue container

static volatile int bufIndex, bufOutdex;


/*
 *  Push a uint8_t from in the FIFO
 */
int FifoPush( const uint8_t data ) {
	if (bufIndex == ((bufOutdex - 1 + MAX_BUF_SIZE) % MAX_BUF_SIZE))
		return 0;
	queue[bufIndex] = data;
	bufIndex = (bufIndex + 1) % MAX_BUF_SIZE;
	return 1;
}

/*
 * Pop a uint8_t from the FIFO
 */
int FifoPop( uint8_t *data ) {
	if (bufIndex == bufOutdex)
		return 0;
	*data = queue[bufOutdex];
	bufOutdex = (bufOutdex + 1) % MAX_BUF_SIZE;
	return 1;
}

