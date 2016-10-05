#include "led_lib.h"
#include "device_manager.h"



INT16S	request_led(INT16U led_num)
{
	return request_device(led_num);	
}

INT16S  release_led(INT16U led_num)
{
	return release_device(led_num);	
}

INT16S led_on(INT16U led_num)
{
	INT16S param = ON;
	return 	request_service(led_num, &param );
}

INT16S led_off(INT16U led_num)
{
	INT16S param = OFF;
	return 	request_service(led_num, &param );
}

INT16S led_switch(INT16U led_num)
{
	INT16S param = SW;
	return 	request_service(led_num, &param );
}