#ifndef _QUEUE_H_
#define _QUEUE_H_
#include "type.h"

#define MAXQUEUESIZE MAXTASKNUM

typedef struct
{
		INT16U q_arr[MAXQUEUESIZE];
		INT16S rear;
		INT16S front;
} queue;

void initQueue(queue *q);
INT16S isEmpty(queue *q);
INT16S isFull(queue *q);
void insertQueue(queue *q, INT16U item);
void deleteQueue(queue *q);
INT16U frontQueue(queue *q);

#endif