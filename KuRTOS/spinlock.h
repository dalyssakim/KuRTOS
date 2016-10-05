#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

typedef struct {
 volatile unsigned int lock;
} spinlock;

#define init_spinlock(x)\
			init_lock(x);
#define spinlock_lock(x)\
			acquire_lock(x);
#define spinlock_unlock(x)\
			release_lock(x);
void init_lock(spinlock *lock_t);
void acquire_lock(spinlock *lock_t);
void release_lock(spinlock *lock_t);
#endif