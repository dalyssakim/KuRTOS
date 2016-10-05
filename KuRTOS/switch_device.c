#include "switch_device.h"
#include "device.h"
#include "device_manager.h"
#include "raw_debug.h"

#include "qsk_bsp.h"


#define EVENT_CONFIG 0b00000001
#define LED_CONFIG 0b00000001
#define CNTR_IPL 0x03


#pragma INTERRUPT Switch0IntHndl;
#pragma INTERRUPT Switch1IntHndl;
#pragma INTERRUPT TimerA4IntHndl;

volatile INT16S S1_STATE ;
volatile INT16S S2_STATE ;
volatile INT16S S3_STATE ;  

/* Nonblock - Switch Device Driver */

void initSwitch1(void);
void initSwitch2(void);
void InitTimerA4(void);

void Switch1IntHndl(void); // SWITCH1
void Switch0IntHndl(void); // SWTICH2
void TimerA4IntHndl(void);

void service_switch1(void *param);
void service_switch2(void *param);
void service_switch3(void *param);

void disableSwitch1(void);
void disableSwitch2(void);
void disableSwitch3(void);

void init_switch_devices(void)
{
	devs[SWITCH1].open = &initSwitch1;
	devs[SWITCH2].open = &initSwitch2;
	devs[SWITCH3].open = &InitTimerA4;


	devs[SWITCH1].service = &service_switch1;
	devs[SWITCH2].service = &service_switch2; /* Change to service2, 3 */
	devs[SWITCH3].service = &service_switch3;


	devs[SWITCH1].close = &disableSwitch1;
	devs[SWITCH2].close = &disableSwitch2;
	devs[SWITCH3].close = &disableSwitch3;

	devs[SWITCH1].dev_support = 1;
	devs[SWITCH2].dev_support = 1;
	devs[SWITCH3].dev_support = 1;
	
	
	S1_STATE = IDLE ;
	S2_STATE = IDLE ;
	S3_STATE = IDLE ;  

}

void service_switch1(void *param)
{
	int *param_int = param;
	*param_int = S1_STATE;
	S1_STATE = IDLE;
}
void service_switch2(void *param)
{

	int *param_int = param;
	*param_int = S2_STATE;
	S2_STATE = IDLE;

}
void service_switch3(void *param)
{

	int *param_int = param;
	*param_int = S3_STATE;
	S3_STATE = IDLE;

}
/* How to do Close? */
void initSwitch2(void)
{
	ilvl0_int0ic = 0;
	ilvl1_int0ic = 1;     
	ilvl2_int0ic = 1;
	pol_int0ic = 0;
	     
}

void initSwitch1(void)
{
	ilvl0_int1ic = 0;
	ilvl1_int1ic = 1;
	ilvl2_int1ic = 1;
	pol_int1ic = 0;
}

void Switch0IntHndl(void){
	S1_STATE = EVENT;
//	display_int_line2(1);
		_asm("fset i");
}


void Switch1IntHndl(void){
	S2_STATE = EVENT;
//	display_int_line2(2);
		_asm("fset i");
}


void InitTimerA4(void){
	ta4 = 0;
		
	_asm("fclr i");
	
	ta4ic |= CNTR_IPL;
	ta4mr = LED_CONFIG;
	
		
	ta4tgl = 0;	
	ta4tgh = 0;
	
	_asm("fset i");
	 
	ta4s = 1;
}

void disableSwitch1(void)
{
}
void disableSwitch2(void)
{
}
void disableSwitch3(void)
{
	ta4s = 0;	
}

void TimerA4IntHndl(void){

	//turn on or offsss
	S3_STATE = EVENT;
		_asm("fset i");
	
}
