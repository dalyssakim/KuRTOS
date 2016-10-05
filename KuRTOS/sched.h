#ifndef _SCHED_H_
#define _SCHED_H_
#include "type.h"

INT16U get_pid();
void wake_up(INT16U tid);
void periodic_wait();
void sleep(INT32U time_t);
TID task_init(void (*program)(void), OS_STK *stbase);
TID task_init_sched_wrr(INT32U priority, void (*program)(void), OS_STK *stbase);
TID task_init_rate_monotonic(INT16U period, INT16U deadline, void (*program)(void), OS_STK *stbase);
#endif