#include "spinlock.h"
#include "clock.h"

void init_lock(spinlock *lock_t)
{
	lock_t->lock = 0;	
}

static int __try_lock(spinlock *lock_t)
{
	unsigned int result;
//	disable_interrupt();
		if( lock_t->lock == 0 ){ lock_t->lock = 1; result = 0;}
		else { result =  1; }
//	enable_interrupt();
	return result;	
}

void acquire_lock(spinlock *lock_t)
{
	while(lock_t->lock == 1){/* Do Nothing Until It's released. */}
}

void release_lock(spinlock *lock_t)
{
	lock_t->lock = 0;	
}