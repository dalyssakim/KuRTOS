#include "msgq_lib.h"
#include "message_queue.h"

INT16S request_channel(INT16S ch)
{
	return open_channel(ch);	
}

INT16S release_channel(INT16S ch)
{
	return close_channel(ch);	
}

INT16S recv_msg(INT16S ch, INT16S *val)
{
	INT16S ret = -1;
	message msg;
	msg.ch = ch;
	msg.buf = -1;
	
	if( ch < 0 ) return -1;
	
	ret = recv_message(&msg);
	*val = msg.buf;
	
	return ret;
}

INT16S send_msg(INT16S ch, INT16S val)
{
	INT16S ret = -1;
	message msg;
	msg.ch = ch;
	msg.buf = val;
	if( ch < 0 ) return -1;
	
	ret = send_message(&msg);
	
	return ret;
}