#ifndef _DEVICE_MANAGER_H_
#define _DEVICE_MANAGER_H_
#include "type.h"
#define MAXDEVNUM 7

enum {
	LCD, RLED, YLED, GLED, SWITCH1, SWITCH2, SWITCH3
};


#define ON	  1
#define OFF	  2
#define SW 3
#define IDLE 0
#define EVENT 1
void init_device_with_support();
void init_devs(void);
INT16S request_device(INT16S dev_num);
INT16S release_device(INT16S dev_num);
INT16S request_service(INT16S dev_num, void * param);
void init_led_devices(void);
void init_switch_devices(void);

#endif