#ifndef _SCHED_WRR_H
#define _SCHED_WRR_H
#include "schedule.h"
ku_tcb * sched_wrr(ku_tcb *tasks, TID current);
void init_sched_wrr(void);
#endif 