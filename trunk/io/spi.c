/*
	file    spi.c
	author  Werner Oswald
	(c)		Werner Oswald
	mail	osiair@osiworx.com
	date    2009-01-01

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    History:

    2009-07-19 initial version for public release



*/
#include "spi.h"
#include "compass.h"
#include "io.h"
#include "../arch/lpc23xx.h"			/* LPC23xx/24xx Peripheral Registers	*/
#include "../arch/sys_config.h"
#include "../nav/mm3parser.h"


/* statistics of all the interrupts */
volatile unsigned long interruptRxStat = 0;
volatile unsigned long interruptOverRunStat = 0;
volatile unsigned long interruptRxTimeoutStat = 0;
volatile unsigned long regValue;

void  __attribute__ ((interrupt("IRQ"))) spi0_isr (void)
{
	regValue = SSP0MIS;
    if ( regValue & SSPMIS_RORMIS )	/* Receive overrun interrupt */
    {
  	interruptOverRunStat++;
  	SSP0ICR = SSPICR_RORIC;		/* clear interrupt */
    }
    if ( regValue & SSPMIS_RTMIS )	/* Receive timeout interrupt */
    {
  	interruptRxTimeoutStat++;
  	SSP0ICR = SSPICR_RTIC;		/* clear interrupt */
    }
    /* please be aware that, in main and ISR, CurrentRxIndex and CurrentTxIndex
    are shared as global variables. It may create some race condition that main
    and ISR manipulate these variables at the same time. SSPSR_BSY checking (polling)
    in both main and ISR could prevent this kind of race condition */
    if ( regValue & SSPMIS_RXMIS )	/* Rx at least half full */
    {
    	interruptRxStat++;		/* receive until it's empty */
    }
    parseMM3();
    VICVectAddr = 0;				//Acknowledge Interrupt

}

void initSSPI0IRQ(void)
{

	  //initialize the interrupt vector
	   VICIntSelect    &= ~VIC_CHAN_TO_MASKLOW(VIC_CHAN_NUM_SSP0);
	   VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_SSP0);
	   VICVectAddr10     = (unsigned long)spi0_isr;
	   VICVectPriority10 = 0x00;
	   VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_SSP0);

}
