#include "device_manager.h"
#include "device.h"
#include "errno.h"
#include "sched.h"
#include "semaphore.h"
#include "spinlock.h"

#define DEVIDLE	-1
device devs[MAXDEVNUM];
static semaphore s;
static spinlock lock;
/*
 * This device driver should only be used for hardware devices.
 * It DOES NOT support multiple tasks on one device.
 * Purposr of use this device manager is to ENSURE that the device is only occupied with ONE TASK at certain time. 
 */

void init_device_with_support()
{
	init_devs();
	init_led_devices();	
	init_switch_devices();
}



void init_devs(void)
{
	int i = 0;
	int j = 0 ;
	for(i = 0; i < MAXDEVNUM; i++)
	{
		devs[i].dev_num = i;
		devs[i].tid = -1; /* NO TASK ATTACHED YET */
		devs[i].dev_support = -1; /* Device Driver For This Device is Not Supported. */	
		devs[i].open = 0;
		devs[i].close = 0;
		devs[i].service = 0;
	}	
	
	initSemaphore(&s, 1);
	init_lock(&lock);
}

INT16S check_device_state(INT16U dev_num)
{
//	if(dev_num < 0) return WRONGDEVNUM;
	if(dev_num >= MAXDEVNUM ) return WRONGDEVNUM;
	if( devs[dev_num].dev_support == -1 ) return WRONGDEVNUM;  /* Change it to NOSUPPORT */

		return 1;	
}



INT16S request_device(INT16S dev_num)
{
	INT16S ret = -1;
	/* Lock */
//	P(&s);

	if( check_device_state(dev_num) && devs[dev_num].tid == DEVIDLE )
	{
		P(&s);
		devs[dev_num].tid = get_pid();	/* Let's change this to get_tid!*/	
			release_lock(&lock);
		(*(devs[dev_num].open))();
		V(&s);
		ret = dev_num;
	}
	else
	{
		ret = DEVICEINUSE;	
	}
//	V(&s);
	/* Unlock */
	return ret;  
}



INT16S release_device(INT16S dev_num)
{
	INT16S ret = -1;

	/* Lock */
//	P(&s);

	if( check_device_state(dev_num) )
	{
		if(	devs[dev_num].tid == get_pid() )
		{
			(*(devs[dev_num].close))();	
			P(&s);
			devs[dev_num].tid = -1;
			V(&s);
			ret = SUCCESS;
		}
		else
		{
			ret = DEVICEINUSE;
		}
	}else
	{
		ret = DEVICEINUSE;	
	}
//	V(&s);
	/* Unlock */
	return ret;
}

INT16S request_service(INT16S dev_num, void * param)
{
	INT16S ret = -1;
	
	/* Lock */
//	P(&s);
	if( check_device_state(dev_num) )
	{
		if(	devs[dev_num].tid == get_pid() )
		{
			(*(devs[dev_num].service))(param);
			ret = SUCCESS;
		}
		else
		{
			ret = DEVICEINUSE;
		}
	}else
	{
		ret = DEVICEINUSE;	
	}
		/* Unlock */
//	V(&s);
	return ret;
	
}