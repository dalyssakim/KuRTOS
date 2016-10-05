#ifndef	_HEAP_H
#define	_HEAP_H
#include "type.h"
#include "schedule.h"


typedef struct
{
	ku_tcb *tcb;
	INT32U	comparator;	
} heapData;

typedef struct 
{
	INT16S heapSize;
	heapData heapArr[MAXTASKNUM];
} heap;


void initHeap(heap *hp);
void insert(heap *hp, ku_tcb *element, INT32U comparator);
ku_tcb * deleteMin(heap *hp);

#endif