#ifndef _MAILBOX_LIB_H_
#define _MAILBOX_LIB_H_
#include "type.h"


INT16S recv_mail(INT16S * val);
void send_mail(INT16U tid, INT16S val);

#endif