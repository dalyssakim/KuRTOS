#include "heap.h"
#include "raw_debug.h"
ku_tcb idle;

/*

For Your Information : 
typedef struct
{
	ku_tcb *tcb;
	INT16U	comparator;	
} headpData;

typedef struct 
{
	INT16S heapSize;
	heapData heapArr[MAXTASKNUM];
} heap;

*/

void initHeap(heap *hp)
{
	idle.tid = -1;
	idle.reserved_run_time = 0;
	hp->heapSize = 0;
	(hp->heapArr[0]).tcb = &idle;
	(hp->heapArr[0]).comparator = 0;
}

void insert(heap *hp, ku_tcb *element, INT32U comparator)
{
	heapData *heapArr; 
	ku_tcb *temp;
	INT16S tempSize = 0;
	INT16S now = 0;
	INT16S root, child;
	hp->heapSize++;
	tempSize = hp->heapSize;	
	heapArr = hp->heapArr;
	heapArr[tempSize].tcb = element;
	heapArr[tempSize].comparator = comparator;
	now = tempSize;
	

	while( (heapArr[now/2]).comparator > comparator)
	{
		heapArr[now] = heapArr[now/2];
		now /= 2;
	}	
	
	heapArr[now].tcb = element;
	heapArr[now].comparator = comparator;
}

ku_tcb * deleteMin(heap *hp)
{
	heapData *heapArr;
	heapData minElement, lastElement; 
	int child, now, lastComp;
	heapArr = hp->heapArr;
	minElement = heapArr[1];
	lastElement = heapArr[hp->heapSize--];
	

	for( now = 1; now*2 <= hp->heapSize; now = child)
	{
		child = now*2;
		

		if(child != hp->heapSize && heapArr[child+1].comparator< heapArr[child].comparator )
		{
			child++;	
		}	
		

		if(lastElement.comparator> heapArr[child].comparator)
		{
			heapArr[now] = heapArr[child];
		}
		else
		{
			/* It's right at the position */

			break;	
		}
	}
	heapArr[now] = lastElement;
	

	return minElement.tcb;
}