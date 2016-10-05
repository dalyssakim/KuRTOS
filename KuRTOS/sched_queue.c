#include "sched_queue.h"

void init_priority_queue(PQUEUE *queue)
{
	initHeap(queue);	
}

void enqueue_rq(PQUEUE *queue, ku_tcb * element)
{
	insert(queue, element, element->period);	
}

void enqueue_pw(PQUEUE *queue, ku_tcb *element)
{
	insert(queue, element, element->reserved_run_time);	
}

void enqueue_nr(PQUEUE *queue, ku_tcb *element)
{
	insert(queue, element, element->priority);
}

ku_tcb * dequeue(PQUEUE *queue)
{
	if(size(queue) == 0) return 0;
	return deleteMin(queue);	
}

ku_tcb *  peek(PQUEUE *queue)
{
	if(size(queue) == 0) return 0;
	return (queue->heapArr[1]).tcb;	
}

INT16S size(PQUEUE *queue)
{
	return queue->heapSize;	
}