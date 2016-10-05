#ifndef CONTEXT_SWITCH_H
#define CONTEXT_SWITCH_H
   

#define SAVE_ISP(x)\
{			_asm("STC FB,$$[FB]",x);}
 
#define RESTORE_ISP(x)\
	{_asm("LDC	$$[FB],	ISP",x);\
	 _asm("POPM	FB");\
	 _asm("POPM	R0,R1,R2,R3,A0,A1");\
	 _asm("REIT"); }
	
	
#endif  