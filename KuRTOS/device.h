#ifndef _DEVICE_H_
#define _DEVICE_H_
#include "sched.h"
#include "device_manager.h"

/* Device Driver ensures that this device is only used in one process */
typedef struct
{
	INT16S dev_num;
	INT16U tid;
	INT16S dev_support;
	void (*open)(void);
	void (*close)(void);
	void (*service)(void *param);	
} device;


extern device devs[MAXDEVNUM];

#endif