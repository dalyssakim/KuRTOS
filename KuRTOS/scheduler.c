#include "schedule.h"
#include "sched_rm.h"
#include "sched_wrr.h"
#include "context_switch.h"
#include "errno.h"
#include "list.h"
#include "clock.h"
#include "qsk_lcd.h"
#include "sched_queue.h"
#include "sched_policy.h"
#include "sched.h"
#include "mailbox.h"

ku_tcb tasks[MAXTASKNUM+1];
static int task_num;
static ku_tcb* (*scheduler)(ku_tcb *tasks, TID current);
static void init_task_struct(void);
int task_init(void (*program)(void), OS_STK *stbase);
INT16S current;
ku_tcb * round_robin(ku_tcb *tasks, TID current);
void wake_up_rr_tasks(ku_tcb *tasks);
INT16U IDLE_TID;
/* Initialize Task internally within the code, and returns TID of the created task. */
TID task_init(void (*program)(void), OS_STK *stbase)
{
	
	#define flag  0x0040// I = 1 (interrupt enable), U = 0 (ISP)

	
	ku_tcb *tcb = &(tasks[task_num]);

	if(task_num == MAXTASKNUM) return ERRMAXTASK;	/* Return ERROR code, ARRAY OUT OF BOUNDARY */

	stbase--; 			
	*stbase--	= (flag & 0x00FF)|
					(((INT32U) program >> 8) & 0x00000F00) | ((flag << 4) & 0xF000);
	*stbase--	= (((INT32U) program) & 0x0000FFFF);

	*stbase--	= (INT16U)0; //A1
	*stbase--	= (INT16U)0; //A0 
	*stbase--	= (INT16U)0; //R3
	*stbase--	= (INT16U)0; //R2
	*stbase--	= (INT16U)0; //R1 
	*stbase--	= (INT16U)0; //R0
	*stbase		= (INT16U)0; //FB

	tcb->isp = stbase;			/* Set the stack base */
	tcb->tid = task_num;		/* Allocate Task Id */
	tcb->state = READY;			/* Initial State, Not yet in the Run Queue */
	tcb->total_execution_time = 0; /* Initialize total execution time */
	tcb->pw_request = FALSE;
	tcb->wk_request = FALSE;
	tcb->policy = RR;
	tcb->timer = 0;
	tcb->mail = -1;
	tcb->mail_request = FALSE;
	task_num++;
	return task_num-1;	
}

INT16U get_pid()
{
	return current;
}

void wake_up(INT16U tid)
{
	tasks[tid].wk_request= TRUE; 	
}

void decrease_quantum(void)
{
	tasks[current].quantum--;
	wake_up_rr_tasks(tasks);	
}
void init_scheduler(INT16S policy)
{
	current = -1;
	task_num = 0;
	IDLE_TID = 0;
	
	
	switch (policy)
	{
		case TS 	: scheduler = &round_robin;									break;
		case RM 	: scheduler = &rate_monotonic;  	init_rate_monotonic(); 	break;
		case RMN	: scheduler = &rate_monotonic_n; 	init_rate_monotonic(); 	break;
		case WRR	: scheduler = &sched_wrr;			init_sched_wrr();		break;
		default 	: scheduler = &rate_monotonic;  	init_rate_monotonic();
	}	

	
}

/* The very first task will be dummy task, the header of task list. */
void init_task_struct(void)
{
	/* ?? */
}

 /* ku_tcb *tasks, TID current, PQUEUE *run_queue, PQUEUE *periodic_wait_queue */
OS_STK * schedule(OS_STK *prev_stk)
{
	OS_STK *stk;
	ku_tcb *curr_t;
	ku_tcb *next_t = (scheduler(tasks, current));
	if(current == -1)
	{
		current = next_t->tid;
		tasks[current].state = RUNNING;

		return tasks[current].isp;
	}
 	else {

	if( next_t->tid != current) 
	{
		/* Current Scheduler is either preempted or consumed all deadline. */
		/*
		 * 1) save isp
		 * 2) current.state = running & start_t !   
		 * 3) stk = current.isp
		 * 4) restore isp
		 */	   
	     if(tasks[current].state == RUNNING){    
		 tasks[current].state = READY;
		 }
		 tasks[current].isp = prev_stk;	 

		 
		 current = next_t->tid;
		 tasks[current].state = RUNNING;
		 stk = tasks[current].isp; 
	}
	else   
	{ 
		/* Nothing happend, return to the current task */
		stk = 0;
	} 
	}
	return stk;
}  


void wake_up_rr_tasks(ku_tcb *tasks)
{
	INT16S i = 0 ;
	
	for(i = 0; i < get_task_num(); i++)
	{	
		if( tasks[i].state == WAIT && tasks[i].timer > 0 )
		{
			tasks[i].timer--;
			if( tasks[i].timer == 0 )
			{
				tasks[i].state = READY;
			}	
		}
		
		if( tasks[i].state == WAIT && tasks[i].wk_request == TRUE)
		{
			tasks[i].state = READY;
			tasks[i].wk_request = FALSE; /* handled */	
		}
	}
}


ku_tcb * round_robin(ku_tcb *tasks, TID current)
{
	INT16S i = 0;
	INT16S curr = current;
	wake_up_rr_tasks(tasks);
	
	if(curr == -1){
		 curr = 1;
	}
	else {
		for( i = 0; i < task_num; i++ ){
			curr = (curr+1)%task_num ;
			if(tasks[curr].state == READY) 	return &(tasks[curr]);		
		}
		
		curr = 0;

	}
		return &(tasks[curr]);

}

INT16S get_task_num(void)
{
	return task_num;	
}

ku_tcb * get_task_arr(void)
{
	return tasks;	
}

void periodic_wait()
{
	/* This will not do any good, if the task is not running on 
	 * Rate Monotonic or Earliest Deadline First Scheduler
	 */
	tasks[current].pw_request= TRUE;
	OS_TASK_SW();
}

void sleep(INT32U time_t)
{
	/* If time_t == 0, then the task will never wake up
	 * until another task intentionally wakes it up
	 */	
	tasks[current].timer = time_t;	
	tasks[current].state = WAIT;
	OS_TASK_SW();
	
}

void send_mail(INT16U tid, INT16S val)
{
	if(tid > 0)
	{
	tasks[tid].mail	= val;
	tasks[tid].mail_request = TRUE;
	}
}

INT16S recv_mail(INT16S * val)
{
	if(tasks[current].mail_request == TRUE)
	{
	*val = tasks[current].mail;
	tasks[current].mail_request = FALSE;
	return 1;	
	}else
	{
	return 0;	
	}
}

void terminate_task(INT16U tid)
{
	if( tid < task_num )
	{
		tasks[tid].state = TERMINATE;	
	}	
}