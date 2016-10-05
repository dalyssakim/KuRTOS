#ifndef _SCHED_RM_H
#define _SCHED_RM_H

#include "schedule.h"
#include "sched_queue.h"
#include "sched_policy.h"

ku_tcb * rate_monotonic(ku_tcb *tasks, TID current);
ku_tcb * rate_monotonic_n(ku_tcb *tasks, TID current);
void periodic_wait_self(ku_tcb *curr_t, PQUEUE *periodic_wait_queue);
void set_up_task_rm(ku_tcb *curr_t);
void init_rate_monotonic();
#endif