#ifndef _LED_LIB_H
#define _LED_LIB_H
#include "type.h"
INT16S	request_led(INT16U led_num);
INT16S 	release_led(INT16U led_num);
INT16S	led_on(INT16U led_num);
INT16S led_off(INT16U led_num);
INT16S led_switch(INT16U led_num);
#endif