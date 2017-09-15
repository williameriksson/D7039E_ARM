/*
 * Stack.h
 *
 *  Created on: 14 sep. 2017
 *      Author: A1201-admin
 */

#ifndef UTILS_STACK_H_
#define UTILS_STACK_H_

#include "stdint.h"

/*
 * Type container for the ques create an instance of queSet_t
*/
typedef struct {
	int bufSize;			// stack index size
	uint32_t index;
	uint32_t outdex;
	int *buf;
} stack_t;


/*
 * Lifo stack
 */
void InitStack( stack_t *stack, int setSize );
int  StackPush( stack_t *stack, int inData );
int  StackPop( stack_t *stack, int *outData );
void FreeStack( stack_t *stack );

#endif /* UTILS_STACK_H_ */
