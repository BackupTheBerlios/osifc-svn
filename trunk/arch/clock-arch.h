/*
	file    clock_arch.h
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


#ifndef CLOCK_ARCH_H
#define CLOCK_ARCH_H

//-----------------------------------------------------------------------------
#include "clock-arch.h"
#include "lpc23xx.h"
#include "sys_config.h"

#include "io/pwmin.h"
//-----------------------------------------------------------------------------

void tc0_isr(void);
void tc1_isr(void);
void tc2_isr(void);
void tc3_isr(void);
void init_timer0(void);
void init_timer1(void);
void init_timer2(void);
void init_timer3(void);

extern volatile unsigned char speaker_on;
extern volatile unsigned char speaker_on_int;

volatile unsigned int timer_counter0;
volatile unsigned int timer_counter1;
volatile unsigned int timer_counter2;
volatile unsigned int timer_counter3;

#define SPEAKER_TONE 	8
#define ON 				1
#define OFF				0

#define TIMER_TEILER0       100000
#define TIMER_TEILER1       144000 //pwmin timer
#define TIMER_TEILER2       100000
#define TIMER_TEILER3       100000

//Timer Control Register
#define TxTCR_COUNTER_ENABLE (1<<0)
#define TxTCR_COUNTER_RESET  (1<<1)

//Interrupt Flags Register TxIR
//Clear interrupt flag by writing 1 to Bit 0
#define TxIR_MR0	(1<<0)
#define TxIR_MR1	(1<<1)
#define TxIR_MR2	(1<<2)
#define TxIR_MR3	(1<<3)
#define TxIR_CR0	(1<<4)
#define TxIR_CR1	(1<<5)
#define TxIR_CR2	(1<<6)
#define TxIR_CR3	(1<<7)

typedef struct {
	unsigned long RTC_Sec;     /* Second value - [0,59] */
	unsigned long RTC_Min;     /* Minute value - [0,59] */
	unsigned long RTC_Hour;    /* Hour value - [0,23] */
	unsigned long RTC_Mday;    /* Day of the month value - [1,31] */
	unsigned long RTC_Mon;     /* Month value - [1,12] */
	unsigned long RTC_Year;    /* Year value - [0,4095] */
	unsigned long RTC_Wday;    /* Day of week value - [0,6] */
	unsigned long RTC_Yday;    /* Day of year value - [1,365] */
} RTCTime;




#define IMSEC		0x00000001
#define IMMIN		0x00000002
#define IMHOUR		0x00000004
#define IMDOM		0x00000008
#define IMDOW		0x00000010
#define IMDOY		0x00000020
#define IMMON		0x00000040
#define IMYEAR		0x00000080

#define AMRSEC		0x00000001  /* Alarm mask for Seconds */
#define AMRMIN		0x00000002  /* Alarm mask for Minutes */
#define AMRHOUR		0x00000004  /* Alarm mask for Hours */
#define AMRDOM		0x00000008  /* Alarm mask for Day of Month */
#define AMRDOW		0x00000010  /* Alarm mask for Day of Week */
#define AMRDOY		0x00000020  /* Alarm mask for Day of Year */
#define AMRMON		0x00000040  /* Alarm mask for Month */
#define AMRYEAR		0x00000080  /* Alarm mask for Year */

#define PREINT_RTC	0x000001C8  /* Prescaler value, integer portion,
				    PCLK = 15Mhz */
#define PREFRAC_RTC	0x000061C0  /* Prescaler value, fraction portion,
				    PCLK = 15Mhz */
#define ILR_RTCCIF	0x01
#define ILR_RTCALF	0x02

#define CCR_CLKEN	0x01
#define CCR_CTCRST	0x02
#define CCR_CLKSRC	0x01





void RTC_ISR (void);
void RTC_Init( void );
void RTC_Start( void );
void RTC_Stop( void );
void RTC_CTCReset( void );
void RTC_Set_Time( RTCTime );
RTCTime RTC_Get_Time( void );
void RTC_Set_Alarm( RTCTime );
void RTC_Set_Alarm_Mask( unsigned long AlarmMask );


#endif

