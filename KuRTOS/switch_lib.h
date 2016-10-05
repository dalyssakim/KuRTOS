#ifndef _SWITCH_LIB_H_
#define _SWITCH_LIB_H_
#include "type.h"
INT16S	request_switch(INT16S switch_num);
INT16S release_switch(INT16S switch_num);
INT16S get_switch(INT16S switch_num, INT16S *val);
#endif