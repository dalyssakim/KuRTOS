#include "qsk_bsp.h"
#include "device_manager.h"
#include "device.h"
#include "led_device.h"

void InitLED1(void);
void InitLED2(void);
void InitLED3(void);
void termLED1(void);
void termLED2(void);
void termLED3(void);
void OnLED(char color);
void OffLED(char color);
void service_red(void *param);
void service_ylw(void *param);
void service_grn(void *param);

void init_led_devices(void)
{
	devs[RLED].open = &InitLED1;
	devs[YLED].open = &InitLED2;
	devs[GLED].open = &InitLED3;


	devs[RLED].service = &service_red;
	devs[YLED].service = &service_ylw; /* Change to service2, 3 */
	devs[GLED].service = &service_grn;


	devs[RLED].close = &termLED1;
	devs[YLED].close = &termLED2;
	devs[GLED].close = &termLED3;

	devs[RLED].dev_support = 1;
	devs[YLED].dev_support = 1;
	devs[GLED].dev_support = 1;

}

void InitLED1(void)
{
	RED_DDR = 1;
	RED_LED = LED_OFF;	
}

void InitLED2(void)
{
	YLW_DDR = 1;
	YLW_LED = LED_OFF;		
}

void InitLED3(void)
{
	GRN_DDR = 1;	
	GRN_LED = LED_OFF;	
}

void termLED1(void)
{
	RED_DDR = 0;	
}

void termLED2(void)
{
	YLW_DDR = 0;	
}

void termLED3(void)
{
	GRN_DDR = 0;	
}

void SwitchLED(char color){
	if( color == RLED && RED_LED == LED_ON ){
		RED_LED = LED_OFF;	
	} else if ( color == YLED && YLW_LED == LED_ON ){
		YLW_LED = LED_OFF;	
	} else if ( color == GLED && GRN_LED == LED_ON ){
		GRN_LED = LED_OFF;	
	} else if( color == RLED && RED_LED == LED_OFF ){
		RED_LED = LED_ON;	
	} else if ( color == YLED && YLW_LED == LED_OFF ){
		YLW_LED = LED_ON;
	} else if ( color == GLED && GRN_LED == LED_OFF ){
		GRN_LED = LED_ON;	
	} else {
		
	}	
}

void OnLED(char color)
{
	if(color == RLED)
	{
		RED_LED = LED_ON;
	}
	else if(color == YLED)
	{
		YLW_LED = LED_ON;
	}
	else if(color == GLED)
	{
		GRN_LED = LED_ON;	
	}
	
}

void OffLED(char color)
{
	if(color == RLED)
	{
		RED_LED = LED_OFF;
	}
	else if(color == YLED)
	{
		YLW_LED = LED_OFF;
	}
	else if(color == GLED)
	{
		GRN_LED = LED_OFF;	
	}
}

void service_red(void *param)
{
	int *param_int = param;
	int com	   = *param_int;
	if( com == ON )
	{
		OnLED(RLED);
	}
	else if( com == OFF )
	{
		OffLED(RLED);	
	}
	else if( com == SW )
	{
		SwitchLED(RLED);
	}
}

void service_ylw(void *param)
{
	int *param_int = param;
	int com	   = *param_int;
	if( com == ON )
	{
		OnLED(YLED);
	}
	else if( com == OFF )
	{
		OffLED(YLED);	
	}
		else if( com == SW )
	{
		SwitchLED(YLED);
	}
}

void service_grn(void *param)
{
	int *param_int = param;
	int com	   = *param_int;
	if( com == ON )
	{
		OnLED(GLED);
	}
	else if( com == OFF )
	{
		OffLED(GLED);	
	}
	else if( com == SW )
	{
		SwitchLED(GLED);
	}
}