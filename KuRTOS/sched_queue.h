#ifndef _SCHED_QUEUE_H
#define _SCHED_QUEUE_H
#include "heap.h"
#define PQUEUE heap

void init_priority_queue(PQUEUE *queue);
void enqueue_rq(PQUEUE *queue, ku_tcb *  element);
void enqueue_pw(PQUEUE *queue, ku_tcb *element);
void enqueue_nr(PQUEUE *queue, ku_tcb *element);;
ku_tcb * dequeue(PQUEUE *queue);
ku_tcb * peek(PQUEUE *queue);
INT16S size(PQUEUE *queue);
#endif