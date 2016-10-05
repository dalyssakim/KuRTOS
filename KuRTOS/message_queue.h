#ifndef _MESSAGE_QUEUE_H_
#define _MESSAGE_QUEUE_H_
#include "type.h"
#include "queue.h"
#define MAXMSGNUM 10
#define MAXMSGQUEUE 10

enum { OPEN, CLOSE, SEND, RECV };

typedef struct 
{
	INT16S ch;				/* Channel Number, This should be same as Device Number */
	INT16S t_count;			/* # of tasks using this queue  */
	INT16U tid[2];
	INT16S d_size;
	INT16U m_size[2];
	queue que[2];
} msg_q;

typedef struct
{
	INT16S ch;
	INT16S service_type;
	INT16S buf;
	
} message;

INT16S open_channel(INT16S ch);
INT16S close_channel(INT16S ch);
INT16S recv_message(message *msg);
INT16S send_message(message *msg);

#endif