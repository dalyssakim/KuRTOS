/*********************************************************************
* DISCLAIMER:                                                        *
* The software supplied by Renesas Technology America Inc. is        *
* intended and supplied for use on Renesas Technology products.      *
* This software is owned by Renesas Technology America, Inc. or      *
* Renesas Technology Corporation and is protected under applicable   *
* copyright laws. All rights are reserved.                           *
*                                                                    *
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS, *
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED 		 *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE *
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND       *
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT      *
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS          *
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL, *
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR        *
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE *
* USE OR APPLICATION OF THIS SOFTWARE.                               *
*********************************************************************/

/*-----------------------------------------------------------------------------
  FILE NAME: qsk_bsp.h
-----------
DESCRIPTION: QSK16C62P Board Support Package.
-----------
    DETAILS: This header file contains useful macros for use with the RTA QSK 

------------------
 Revision History
------------------
   1.2 Aug 3, 2004
       - Changed XIN_FREQ to f1_CLK_SPEED 
   1.0 Jan 1, 2004
       Initial Version
-----------------------------------------------------------------------------*/

#include "sfr62p.h"  		// M16C/62P special function register definitions
#include "qsk_lcd.h"  		// SKP LCD function definitions

/* Peripheral Clock Speed */
#define	f1_CLK_SPEED		24000000		// Peripheral Clock Speed - set up in mcu_init()
#define XIN_FREQ 			f1_CLK_SPEED

/* Switches */
#define	S1 				p8_3 
#define S2 				p8_2 
#define S3 				p8_1 
#define S1_DDR			pd8_3
#define S2_DDR			pd8_2
#define S3_DDR			pd8_1

/* LEDs */
#define	RED_LED			p8_0
#define	YLW_LED			p7_4
#define	GRN_LED			p7_2

#define	RED_DDR 		pd8_0		// LED port direction register
#define	YLW_DDR 		pd7_4
#define	GRN_DDR 		pd7_2

/********************************************************************************/
/* Macro Definitions 															*/
/********************************************************************************/
#define LED_ON      	0
#define LED_OFF     	1
#ifndef TRUE
#define TRUE    		1
#endif
#ifndef FALSE
#define FALSE   		0
#endif

#define ENABLE_IRQ   	{_asm(" FSET I");}
#define DISABLE_IRQ		{_asm(" FCLR I");}

// Use these macros for switch inputs. 
#define ENABLE_SWITCHES {S1_DDR = 0; S2_DDR = 0; S3_DDR = 0;}

// Use these macros to control the LEDs.  
#define LED(led, state) ((led) = !state)
#define ENABLE_LEDS {RED_LED = LED_OFF; YLW_LED = LED_OFF; GRN_LED = LED_OFF; RED_DDR = 1; YLW_DDR = 1; GRN_DDR = 1; }
