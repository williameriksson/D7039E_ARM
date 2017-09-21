/*
 * Stack.c
 *
 *  Created on: 14 sep. 2017
 *      Author: Sam
 */


#include "Stack.h"
#include "stdlib.h"

/*
 *
 */
void InitStack( stack_t *stack, int setSize ) {

	stack->buf 		= ( int * )malloc( (setSize) * sizeof( int ) );
	stack->index 	= 0;
	stack->outdex 	= 0;
	stack->bufSize 	= setSize;
}

/*
 *  Push a int in the Lifo stack
 */
int StackPush( stack_t *stack, int inData ) {

	if ( stack->index == stack->bufSize )  // stack overflow
		return 0;

	stack->buf[stack->index] = inData;
	stack->outdex = stack->index;
	stack->index++;
	return 1;
}

/*
 * Pop a int from the Lifo stack
 */
int StackPop( stack_t *stack, int *outData ) {

	if ( !stack->index )	// stack empty
		return 0;

	*outData = stack->buf[stack->outdex];
	stack->index = stack->outdex;

	if( stack->outdex )
		stack->outdex--;

	return 1;
}


/*
 *  Use this when you're done with the fifo
 *  othervize you're going to have a bad time. ( memory leakages! )
 */
void FreeStack( stack_t *stack ) {
	free( stack->buf );
	stack->buf = 0;
	stack->index = 0;
	stack->bufSize = 0;
}



// test case
/*InitStack( &stack, 4 );

StackPop( &stack, &queVar );	// does nothing

StackPush( &stack, 2 );
StackPush( &stack, 3 );
StackPush( &stack, 4 );
StackPush( &stack, 5 );
StackPush( &stack, 25 );		// will not be inserted


queVar = stack.buf[0];
queVar = stack.buf[1];
queVar = stack.buf[2];
queVar = stack.buf[3];
queVar = stack.buf[4];		// nothing


StackPop( &stack, &queVar );
StackPop( &stack, &queVar );
StackPop( &stack, &queVar );
StackPop( &stack, &queVar );
StackPop( &stack, &queVar );	// does nothing


LifoPush( &que, 264 );
LifoPush( &que, 364 );
LifoPush( &que, 224 );
LifoPush( &que, 264 );
LifoPop( &que, &queVar );
LifoPush( &que, 123 );
LifoPush( &que, 695 );			// does nothing


LifoPop( &que, &queVar );
LifoPop( &que, &queVar );
LifoPop( &que, &queVar );
LifoPop( &que, &queVar );		// 123

	FreeLifo( que );
	*/
