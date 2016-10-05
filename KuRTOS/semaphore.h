#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "queue.h"
#include "type.h"
#include "clock.h"
 
typedef struct {
	INT16S count;
	queue q;
} semaphore;
 
 void initSemaphore(semaphore *s, INT16S count);
 void P(semaphore *s);
 void V(semaphore *s);

#endif