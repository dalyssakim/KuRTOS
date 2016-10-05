#include "qsk_bsp.h"
#include "schedule.h"
#include "context_switch.h"
#include "qsk_lcd.h"
#include "clock.h"
#define TIME_CONFIG 0b01000100
#define EVENT_CONFIG 0b00000001 

#define CNTR_IPL 0x03

void TimerA1IntHndl(void);

#pragma INTERRUPT TimerA1IntHndl;

INT32U t;
volatile INT16S ctx_flag; 
void init_tick();
unsigned long get_time()
{
	return t;	
}

void init_tick()
{
	t = 0;	
	ctx_flag = 1;
}


void disable_interrupt()
{
//	GET_INT_STATUS(iflg);	
//	DISABLE_INTS;
	ctx_flag = 0;
}

void enable_interrupt()
{ 
//	if(iflg)
//	 ENABLE_INTS;
	ctx_flag = 1;	
}

void InitTimerA0(void){
	ta0 = 3000-1;
		
	_asm("fclr i");
	
	ta0ic |= CNTR_IPL;
	ta0mr = TIME_CONFIG;

	
	_asm("fset i");
	 
	ta0s = 1;

		
}


/*
 * Try first with scheduler every second.
 */
void InitTimerA1(void){
	ta1 = 100-1;
		
	_asm("fclr i");
	
	ta1ic |= CNTR_IPL;
	ta1mr = EVENT_CONFIG;
	
		
	ta1tgl = 0;	
	ta1tgh = 1;
	
	_asm("fset i");
	 
	ta1s = 1;
}


 
 void TimerA1IntHndl(void)
 {
	OS_STK *stk;
	t++; 
	if( ctx_flag == 1){
		SAVE_ISP(stk);
		stk = schedule(stk);
		if( stk != 0 )
		{  RESTORE_ISP(stk); }
	}
	else
	{
		decrease_quantum();	
	}
	 
 } 