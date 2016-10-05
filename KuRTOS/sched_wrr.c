#include "sched_wrr.h"
#include "sched_queue.h"
#include "raw_debug.h"
#include "sched.h"
INT32U calc_quantum_sp(INT32U priority);

static PQUEUE active_rq;
static PQUEUE expired_rq;
static PQUEUE *cq;
static PQUEUE *eq;

/*
*	This is WEIGHTED ROUND ROBIN!
*/
void init_sched_wrr()
{
	init_priority_queue(&active_rq); /* Initialize Run Queue */
	init_priority_queue(&expired_rq); /* Initialize Periodic Wait Queue */	
	cq = &active_rq;
	eq = &expired_rq;
}

INT32U calc_quantum_sp(INT32U priority)
{
	INT32U quantum;
	
	if(priority<120)
	{
		quantum = (INT32U)((140-priority)*20);
	}
	else
	{
		quantum = (INT32U)((140-priority)*20);	
	}
	return quantum;
}

TID task_init_sched_wrr(INT32U priority, void (*program)(void), OS_STK *stbase)
{
	TID tid = -1;
	ku_tcb *tasks = get_task_arr();
	ku_tcb *tcb = &(tasks[get_task_num()]);

	tcb->period = 0;		/* Allocate Period */
	tcb->deadline = 0;	/* Allocate Deadline */
	tcb->reserved_run_time = 1; /* Ready For Scheduling */
	tcb->priority = priority; 	/* Initialize Period */

//	tcb->quantum = calc_quantum_sp(priority);	/* quantum will be consumed when this task is running */
	tcb->quantum = 5-priority;	/* quantum will be consumed when this task is running */
	tid = task_init(program, stbase);
	if( tid != IDLE_TID ){	enqueue_nr(cq, &(tasks[tid])); }/* Make this task Ready! */

	return tid;
}

void wake_up_tasks(ku_tcb *tasks)
{
	INT16S i = 0 ;
	
	for( i = 0; i < get_task_num(); i++ )
	{
			
		if( tasks[i].state == WAIT && tasks[i].timer > 0 )
		{
			tasks[i].timer--;
			if( tasks[i].timer == 0 )
			{
				tasks[i].state = READY;
				enqueue_nr(eq, &(tasks[i]));
			}	
		}
		
		if( tasks[i].wk_request == TRUE )
		{
			enqueue_nr(eq, &(tasks[i])); 
			tasks[i].wk_request = FALSE;	
		}	
	}	
}

ku_tcb * sched_wrr(ku_tcb * tasks, TID current)
{
	PQUEUE *temp;
	ku_tcb *next_t;

		/* This will wake up the processes reqested to be waken up */
		wake_up_tasks(tasks);

		if( current == -1 )
		{
			next_t = dequeue(cq);
			return next_t;	
		}

		tasks[current].quantum--;
		if(tasks[current].quantum > 0)
		{
			 return &tasks[current];
		}
			
		if(tasks[current].quantum == 0){
			//tasks[current].quantum = calc_quantum_sp(tasks[current].priority);
			tasks[current].quantum = 5-tasks[current].priority;
			enqueue_nr(eq, &(tasks[current])); 
		}
		if(size(cq) == 0) 
		{
			/* Swap Off The Run Queues */
			temp = cq;
			cq = eq;
			eq = temp;	
			/* Now Empty Queue is Expired Queue & Full One Active Queue */
		}

			next_t = dequeue(cq);
			return 	next_t;
	
}