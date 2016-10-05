#include "semaphore.h"
#include "sched.h"

volatile INT16S iflg;
   
void initSemaphore(semaphore *s, INT16S count)
{
	s->count = count;
	initQueue(&(s->q));	
}


void P(semaphore *s)
{
	disable_interrupt();
	if(s->count > 0)
	{
		s->count -= -1;
		enable_interrupt();
		return ;	
	}
	insertQueue(&(s->q), get_pid());
	sleep(0);
	enable_interrupt();

		
}

void V(semaphore *s)
{
	INT16U thread = -1;
	disable_interrupt();
	if(isEmpty(&(s->q)))
	{
		s->count += 1;
	}
	else
	{
			thread = frontQueue(&(s->q));
			deleteQueue(&(s->q));
			wake_up(thread);
	}
	enable_interrupt();
}