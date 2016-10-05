#include "switch_lib.h"
#include "device_manager.h"


INT16S	request_switch(INT16S switch_num)
{
	if( switch_num < SWITCH1 || switch_num > SWITCH3 ) return -1;
		return request_device(switch_num);	
}
INT16S release_switch(INT16S switch_num)
{
		if( switch_num < SWITCH1 || switch_num > SWITCH3 ) return -1;
		return release_device(switch_num);	
}
INT16S	get_switch(INT16S switch_num, INT16S *val)
{
		return 	request_service(switch_num, val);
}

