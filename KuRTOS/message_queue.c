#include "message_queue.h"
#include "device_manager.h"
#include "device.h"
#include "semaphore.h"
#include "sched.h"
#include "spinlock.h"
#define AVAIL -1
static semaphore s;
static spinlock lock;
static msg_q mq[MAXMSGQUEUE];	
static INT16S mq_size;
void init_message_queue_devices(void)
{
	int i = 0;
	mq_size = 0;
	for(i = 0; i < MAXMSGQUEUE; i++)
	{
		mq[i].ch = AVAIL;
		mq[i].t_count = 0;
		mq[i].d_size = 0;
		mq[i].m_size[0] = 0;
		mq[i].m_size[1] = 0;
		mq[i].tid[0] = 0;
		mq[i].tid[1] = 0;
		mq[i].que[0].front = -1;
		mq[i].que[0].rear = -1;
		mq[i].que[1].front = -1;
		mq[i].que[1].rear = -1;	
	}
	/* Initialize Semaphore */
	initSemaphore(&s, 1);
	init_lock(&lock);
}

/* 
 * MESSAGE QUEUE OPENING CHECK LIST
 * 1) already opened and you already in --> return -2
 * 2) already opened but you not in --> ok, register this
 * 3) already opened and full --> return -1
 * 4) not opened --> ok, create one and register this
 */
 #define EMPTYOPEN 1
 #define HALFOPEN 2
 #define ALREADY -2
 #define OCCUPIED -1
INT16S channel_stalker(INT16S ch, INT16U tid)
{
	INT16S i = 0;
	for( i = 0; i < MAXMSGQUEUE; i++)
	{
		if(	mq[i].ch == ch )
		{
			if( mq[i].t_count == 1 && (mq[i].tid[0] == tid || mq[i].tid[1] == tid) )
			{ return ALREADY; /* already opened and you in */ }
			else if( mq[i].t_count == 1 && (mq[i].tid[0] != tid && mq[i].tid[1] != tid) )
			{ return HALFOPEN; } /* Someone opened it already, but one room is available */
			else if ( mq[i].t_count == 2)
			{ return OCCUPIED; } /* Two are already using it */
		
		}
	}
	
	return EMPTYOPEN; /* No channel, create one */
}

INT16S find_index_by_ch(INT16S ch)
{
	INT16S i = 0;
	for(i = 0; i < MAXMSGQUEUE; i++)
	{
		if( mq[i].ch == ch ){ return i;	}
	}	
	
	return -1;
}

void init_emptry_channel(INT16S ch)
{
	INT16S i = 0;
	for( i = 0; i < MAXMSGQUEUE; i++)
	{
		if( mq[i].ch == -1 )
		{
			mq[i].ch = ch;
			return ;
		} 	
	}
};

void register_task(INT16S ch, INT16S type, INT16U tid)
{
	INT16S i = -1;
	if( type == EMPTYOPEN )
	{
		init_emptry_channel(ch);
		i = find_index_by_ch(ch);
		mq[i].tid[0] = tid;	
		mq[i].t_count = 1;
	}
	else if( type == HALFOPEN ){
		i = find_index_by_ch(ch);
	if(mq[i].tid[0] > 0){ mq[i].tid[1] = tid; }
	else				{ mq[i].tid[0] = tid; }
	mq[i].t_count = 2; 
	}
}

INT16S open_channel(INT16S ch)
{
	INT16S i = 0;
	INT16S ret = -1;
	INT16S scary_stalker = -1;
	INT16U tid = -1;
	//	P(&s);
		acquire_lock(&lock);
		tid = get_pid();
		scary_stalker = channel_stalker(ch, tid);
	if( scary_stalker == EMPTYOPEN)
	{
		register_task(ch, scary_stalker , tid);
		mq_size++;
		ret =  ch;
	}
	else if( scary_stalker == HALFOPEN )
	{
		register_task(ch, scary_stalker , tid);	
		ret = ch;
	}
	else
	{
		ret = 9;		
	}
		V(&s);
	return ret;
}



#define CLOSED 1
#define PREVILAGE -9
/* 
 * MESSAGE QUEUE CLOSING CHECK LIST
 * 1) already opened and you already in --> return CLOSED
 * 2) already opened but you not in --> return PREVILAGE - you don't have any previlage to close this message queue
 * 3) not opened --> return PREVILAGE 
 */
INT16S close_channel(INT16S ch)
{
	INT16S index = -1;
	INT16S ret = -1;
//	P(&s);
	acquire_lock(&lock);
	index = find_index_by_ch(ch);
	if(index == -1 ) ret = PREVILAGE;
	else if( mq[index].tid[0] == get_pid() || mq[index].tid[1] == get_pid() )
	{
		/* Reset Channel */
		mq[index].ch = -1;
		mq[index].tid[0] = 0;
		mq[index].tid[1] = 0;
		mq[index].t_count = 0;
		mq[index].que[0].front = -1;
		mq[index].que[0].rear = -1;
		mq[index].que[1].front = -1;
		mq[index].que[1].rear = -1;
		mq_size--;
		ret = CLOSED;	
	}
	else
	{
		ret = PREVILAGE;	
	}
//	V(&s);
	release_lock(&lock);
	return ret;
}

INT16S recv_message(message *msg)
{
	INT16S ch = -1;
	INT16S index = -1;
	INT16U tid = -1;
	INT16S ret = -1;
	//P(&s);
		acquire_lock(&lock);
	tid = get_pid();
	ch = msg->ch;
	index = find_index_by_ch(ch);
	if(index == -1) ret = -11;
	else
	{
		if(mq[index].tid[0] == tid && mq[index].t_count == 2)
		{
			/* Read Process */
			if( isEmpty(&(mq[index].que[0])) ){
				ret = 0;
			}
			else{
			msg->buf = frontQueue(&(mq[index].que[0]));
			deleteQueue(&(mq[index].que[0]));
				ret = 1;
			}
				
		}
		else if(mq[index].tid[1] == tid && mq[index].t_count == 2)
		{
			
			/* Read Process */
			if( isEmpty(&(mq[index].que[1])) ){
				ret = 0;
			}
			else{
			msg->buf = frontQueue(&(mq[index].que[1]));
						deleteQueue(&(mq[index].que[1]));
			
				ret = 1;
			}
		}
		else
		{
			ret = PREVILAGE;	
		}
	}
//	V(&s);
	release_lock(&lock);
	return ret;
		
}


INT16S send_message(message *msg)
{
	INT16S ch = -1;
	INT16S index = -1;
	INT16U tid = -1;
	INT16S ret = -1;
	INT16S val = -1;
	
	//P(&s);
		acquire_lock(&lock);
	ch = msg->ch;
	val = msg->buf;
	tid = get_pid();
	index = find_index_by_ch(ch);
	if(index == -1) ret =  -10;
	else
	{
		if(mq[index].tid[0] == tid && mq[index].t_count == 2)
		{
			/* Write Process */
			if(isFull(&(mq[index].que[1])) ){
				ret = 0;
			}
			else{
			insertQueue(&(mq[index].que[1]), val);
				ret = 1;
			}
				
		}
		else if(mq[index].tid[1] == tid && mq[index].t_count == 2)
		{
			
			/* Write Process */
			if(isFull(&(mq[index].que[0])) ){
				ret = 0;
			}
			else{
			insertQueue(&(mq[index].que[0]), val);
			
				ret = 1;
			}
		}
		else
		{
			ret = PREVILAGE;	
		}
	}
	release_lock(&lock);	
//	V(&s);
	return ret;	
}