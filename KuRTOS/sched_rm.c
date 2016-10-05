#include "sched_rm.h"
#include "errno.h"
#include "qsk_lcd.h"
#include "clock.h"
#include "type.h"
#include "sched.h"

PQUEUE rq;
PQUEUE pwq;
void set_up_task_rm(ku_tcb *curr_t);
void deadline_miss(ku_tcb *curr_t, PQUEUE *run_queue);
void fetch_task(void);

void init_rate_monotonic()
{
	init_priority_queue(&rq); /* Initialize Run Queue */
	init_priority_queue(&pwq); /* Initialize Periodic Wait Queue */	
}

TID task_init_rate_monotonic(INT16U period, INT16U deadline, void (*program)(void), OS_STK *stbase)
{
	TID tid = -1;
	ku_tcb *tasks = get_task_arr();
	ku_tcb *tcb = &(tasks[get_task_num()]);
	tcb->period = period;		/* Allocate Period */
	tcb->deadline = deadline;	/* Allocate Deadline */
	tcb->reserved_run_time = 1; /* Ready For Scheduling */
	tcb->quantum = deadline;	/* quantum will be consumed when this task is running */
	tcb->policy = RMN;
	tid = task_init(program, stbase);
	if( tid != IDLE_TID )	enqueue_rq(&rq, &(tasks[tid])); /* Make this task Ready! */

	return tid;
}
  
ku_tcb * rate_monotonic(ku_tcb *tasks, TID current)
{
	ku_tcb * curr_t;
	ku_tcb * tcb_min;
	ku_tcb * first_wait_t;
	ku_tcb * temp_t;
	int i = 0;
	
	if(current == -1)
	{
		tcb_min = dequeue(&rq);
		return tcb_min;
	}
	if( current != IDLE_TID ){

	/* Comes Only It's not Idling */	
	curr_t = &(tasks[current]);
	tasks[current].quantum--; /* Decrease Quantum */
	
	if(curr_t->pw_request == TRUE)
	{
		periodic_wait_self(curr_t, &pwq);
		curr_t->pw_request = FALSE; /* OK, Handled */
		/* This means, just dequeue the next task. CURRENT ONE IS ALREADY IN PERIODIC WAIT QUEUE */
	}else if(curr_t->quantum <= 0 )
	{
		/* Deadline Miss */	  
		periodic_wait_self(curr_t, &pwq);  
	}
	else if(curr_t->quantum > 0 )
	{
		enqueue_rq(&rq, &(tasks[current]));
	}
	}
	/* Fetch Currently Waiting Periodic Wait Task To RQ */
	if( size(&pwq) > 0 ) 
	{
		fetch_task();
	}
	
	
	if( size(&rq) == 0 ) return &(tasks[IDLE_TID]);
	tcb_min = peek(&rq);

	
	if( tcb_min != 0 )	tcb_min = dequeue(&rq);
	return tcb_min;

}

void fetch_task(void)
{
	ku_tcb *temp;

	if( size(&pwq) == 0 ) return;  

		temp = peek(&pwq);
		if( temp == 0 ) return;
	while((temp->reserved_run_time <= get_time() ))
	{
		/* If a task in periodic wait queue is time to run again,
			dequeue it from pwq and enque to rq */
     	temp = dequeue(&pwq);
		enqueue_rq(&rq, temp);
		temp = peek(&pwq);
		if( temp == 0 ) break;
	}

	return ;
}

void wake_up_rm_tasks(ku_tcb *tasks)
{
	INT16S i = 0 ;
	INT16S temp = 0;
	for(i = 0; i < get_task_num(); i++)
	{
		if( i != IDLE_TID ){
		if( (tasks[i].state == WAIT) && (tasks[i].wk_request == TRUE) )
		{
			tasks[i].state = READY;
			tasks[i].wk_request = FALSE;	
		}
		
		if( (tasks[i].state == WAIT) && (tasks[i].timer > 0) )
		{
			tasks[i].timer--;
			if( tasks[i].timer == 0 )
			{
				tasks[i].state = READY;
			}	
		}
		}
	}
} 


ku_tcb * rate_monotonic_n(ku_tcb *tasks, TID current)
{ 
	unsigned long E, E_R, CURRENT_TIME;
	ku_tcb *candidate[MAXTASKNUM];
	ku_tcb *next_task = 0;  
	INT16S candidate_num = 0; 
	INT16S i = 0;
	
	
	wake_up_rm_tasks(tasks);

	if( current != IDLE_TID ){
	tasks[current].quantum--; 

	if( tasks[current].quantum == 0 || tasks[current].pw_request == TRUE )
	{	
		tasks[current].pw_request = FALSE;
		set_up_task_rm(&(tasks[current]));
	}
	}
	
	for( i = 0; i < get_task_num(); i++ )
	{
		if( i != IDLE_TID )
		{
			if( tasks[i].reserved_run_time <= get_time()
				&& (tasks[i].state == READY || tasks[i].state == RUNNING ) )
				{
					candidate[candidate_num] = &(tasks[i]);
					candidate_num++;	
				}
		}
	}

	
	if(candidate_num != 0)
	{
		next_task = candidate[0];
		for( i = 0; i < candidate_num; i++)
		{ 
			if(	next_task->period > candidate[i]->period )
			{ next_task = candidate[i]; }
		}
	}
	else
	{
		return &(tasks[IDLE_TID]);
	}
	return next_task;    
} 

void periodic_wait_self(ku_tcb *curr_t, PQUEUE *periodic_wait_queue)
{
	set_up_task_rm(curr_t);
	enqueue_pw(periodic_wait_queue, curr_t);	
}

void set_up_task_rm(ku_tcb *curr_t)
{
	curr_t->reserved_run_time = curr_t->reserved_run_time + curr_t->period;
	curr_t->total_execution_time += (curr_t->deadline - curr_t->quantum);
	curr_t->quantum = curr_t->deadline;
	
}

void deadline_miss(ku_tcb *curr_t, PQUEUE *run_queue)
{
	set_up_task_rm(curr_t);
	enqueue_rq(run_queue, curr_t);	
	
}
