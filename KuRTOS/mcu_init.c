// tab space = 4
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
  FILE NAME: mcu_init.c
-----------
DESCRIPTION: System clock and processor mode initilization
-----------
    DETAILS: For M16C/62P

------------------
 Revision History
------------------
   1.0 Sept 15, 2004
       Initial Version

-----------------------------------------------------------------------------*/
//#include "skp_bsp.h"
#include "sfr62p.h"  		// M16C/62P special function register definitions

/**************************************************************************
Name       : mcu_init()   
Parameters : none                   
Returns    : nothing      
Description: The starter kit startup file initializes the clock circuit
             to the main crystal with a divide by 1.  This function also sets
			 the main clock to divide by 1 in case the SKP startup file is not 
			 used.  It then enables the PLL 
     
***************************************************************************/
void mcu_init(void)
{
	 unsigned int count = 20000;
  
	/* configure clock for divide by 1 mode */
	prc0 =1;					/* enable access to clock registers */
	cm1 = 0x20;					/* set CM16,CM17 divide ratio to 1, 
									main clock on in high drive no PLL*/
	cm06 = 0;					/* set divide ratio to 1 */		
	
    /* configure and switch main clock to PLL at 24MHz - comment this section
		out if PLL operation is not desired								*/
   
	prc1 = 1;					/* allow writing to processor mode register */
	pm20 = 1;					/* set SFR access to 2 wait which is required for
   									operation greater than 16 MHz */
	prc1 = 0;					/* protect processor mode register */

	plc0= 0x91;					// enable PLL (2X) and turn on

	while(count > 0) count--;   /* wait for PLL to stabilize (20mS maximum, 240,000 cycles @12Mhz)
                                   this decrement with no optimization is 12 cycles each */
    cm11 = 1;                   /* switch to PLL */
    prc0 = 0;   				/* protect clock control register */
	
	prc1 = 1;
	pm10 = 1;		// enable data flash area
	prc1 = 0;
}
