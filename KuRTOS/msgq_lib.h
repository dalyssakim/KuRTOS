#ifndef _MSGQ_LIB_H_
#define _MSGQ_LIB_H_
#include "type.h"
INT16S request_channel(INT16S ch);
INT16S release_channel(INT16S ch);
INT16S recv_msg(INT16S ch, INT16S *val);
INT16S send_msg(INT16S ch, INT16S val);
#endif