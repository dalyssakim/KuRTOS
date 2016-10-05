#include "queue.h"



void initQueue(queue *q)
{
	q->rear = -1;
	q->front = -1;	
}

INT16S isEmpty(queue *q)
{
	return (q->front == -1 && q->rear == -1);	
}

INT16S isFull(queue *q)
{
	return (q->rear +1)%MAXQUEUESIZE == q->front ? 1 : 0;	
}

void insertQueue(queue *q, INT16U item)
{
	if( isFull(q) ){
		return ; /* NO Room */	
	}
	else
	{
		if( isEmpty(q) )
		{
			q->front = q->rear = 0;
		}
		else
		{
			q->rear = (q->rear + 1)%MAXQUEUESIZE;	
		}	
		
		q->q_arr[q->rear] = item;
	} 	
}

void deleteQueue(queue *q)
{
	if(isEmpty(q))
	{
		return ;	
	}else if( q->front == q->rear)
	{
		q->front = q->rear = -1;	
	}
	else
	{
		q->front = (q->front + 1)%MAXQUEUESIZE;	
	}
}

INT16U frontQueue(queue *q)
{
	if( q->front == -1 )
	{
		return -1;	
	}	
	return q->q_arr[q->front];
}