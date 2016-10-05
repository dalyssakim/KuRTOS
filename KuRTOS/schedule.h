#ifndef SCHEDULE_H
#define SCHEDULE_H
#include "list.h"
#include "type.h"


typedef enum T_STATE {START, READY, RUNNING, WAIT, TERMINATE} T_STATE;

typedef struct
{
	struct list_head t_list;

	OS_STK *isp;
	TID tid;
	T_STATE state;	

	INT32U priority;
	INT32U period;
	INT32U deadline;
	INT32U reserved_run_time;
	volatile INT32U quantum;
	INT32U total_execution_time;
	INT32U policy;
	INT32U timer;
	INT16S mail;
	BOOLEAN mail_request;
	BOOLEAN pw_request;
	BOOLEAN wk_request;
} ku_tcb;



extern INT16S current;
void display_tid(int tid);
void init_scheduler(int policy);
INT16S get_task_num(void);
ku_tcb * get_task_arr(void);
OS_STK * schedule(OS_STK *prev_stk);
ku_tcb * schedule_t(void);
ku_tcb * get_prev_t(void);
void decrease_quantum(void);
#endif