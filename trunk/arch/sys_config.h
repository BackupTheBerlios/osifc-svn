/*
	file    sys_config.h
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
#ifndef SYS_CONFIG_H
#define SYS_CONFIG_H

#include "lpc23xx.h"
#include "lpc_util_defs.h"


#ifndef BIT
#define BIT(n)              (1 << (n))
#endif

#ifndef NBIT
#define NBIT(n)             (0 << (n))
#endif

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif



#define WDOG()

//PLL setup values are computed within the LPC include file
//It relies upon the following defines
//PLL input is the XTAL at FOSC
//FCCO is FOSC * PLL_MUL * 2 / PLL_DIV
//

/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* PLL input Crystal frequency range 4KHz~20MHz. */
#define FOSC  12000000
#define PLL_MUL             (12)        //PLL Multiplier
#define PLL_DIV             (1)         //PLL Divider
#define CCLK_DIV            (4)         //PLL out -> CPU clock divider


//Peripheral Bus Speed Divider
#define PBSD                1           //MUST BE 1, 2, or 4
#define FCCO          		(FOSC * PLL_MUL * 2 / PLL_DIV) //CC Osc. Freq.
#define CCLK                ( FCCO / CCLK_DIV ) //CPU Clock Freq.
#define PCLK                (CCLK / PBSD) //Peripheral Bus Clock Freq.

#define CCLKCFG_CCLKSEL_VAL ((CCLK_DIV -1 ) << 0UL )

//Do some value range testing
//TODO: check Minimum for LPC23xx/24xx
#if ((FOSC < 10000000) || (FOSC > 25000000))
#error Fosc out of range (10MHz-25MHz)
#error correct and recompile
#endif

#if ((CCLK > 72000000))
//TODO: check Minimum for LPC23xx/24xx
#error cclk out of range (10MHz-72MHz)
#error correct PLL_MUL and recompile
#endif

#if ((CCLK < 10000000))
//TODO: check Minimum for LPC23xx/24xx
#error cclk out of range (10MHz-72MHz)
#error correct PLL_MUL and recompile
#endif
/* APB clock frequency , must be 1/2/4 multiples of ( Fcclk/4 ). */
/* If USB is enabled, the Minimum APB must be greater than 16Mhz */
//USB is not supported by osiFC
#if USE_USB
#define Fpclk	(CCLK / 2)
#else
#define Fpclk	(CCLK / 1)
#endif


//"The resulting frequency must be in the range of 275 MHz to
//550 MHz." (Manual p. 36)
#if ((FCCO < 275000000) || (FCCO > 550000000))
#error Fcco out of range (275MHz-550MHz)
#error internal algorithm error
#endif

#if ((PBSD != 1) && (PBSD != 2) && (PBSD != 4))
#error Pheripheal Bus Speed Divider (PBSD) illegal value (1, 2, or 4)
#endif



#endif
