/*
	file    clock-arch.c
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


#include "clock-arch.h"

volatile unsigned char speaker_on = 0;
volatile unsigned char speaker_on_int = 0;

volatile unsigned int timer_counter0 = 0;
volatile unsigned int timer_counter1 = 0;
volatile unsigned int timer_counter2 = 0;
volatile unsigned int timer_counter3 = 0;

//Timer0 Compare-Match Interrupt Handler (ISR)
void __attribute__ ((interrupt("IRQ"))) tc0_isr(void)
{
	timer_counter0++;
	T0IR = TxIR_MR0; //Clear interrupt flag by writing 1 to Bit 0
	VICVectAddr = 0;       //Acknowledge Interrupt (rough?)
}
//Timer0 Compare-Match Interrupt Handler (ISR)
void __attribute__ ((interrupt("IRQ"))) tc1_isr(void)
{
	timer_counter1++;
	T1IR = TxIR_MR0; //Clear interrupt flag by writing 1 to Bit 0
	VICVectAddr = 0;       //Acknowledge Interrupt (rough?)
}

//Timer0 Compare-Match Interrupt Handler (ISR)
void __attribute__ ((interrupt("IRQ"))) tc2_isr(void)
{
	timer_counter2++;
	T2IR = TxIR_MR0; //Clear interrupt flag by writing 1 to Bit 0
	VICVectAddr = 0;       //Acknowledge Interrupt (rough?)
}

//Timer0 Compare-Match Interrupt Handler (ISR)
void __attribute__ ((interrupt("IRQ"))) tc3_isr(void)
{
	timer_counter3++;
	T3IR = TxIR_MR0; //Clear interrupt flag by writing 1 to Bit 0
	VICVectAddr = 0;       //Acknowledge Interrupt (rough?)
}

//Setup Timer0 Compare-Match Interrupt
//no prescaler timer runs at cclk = FOSC*PLL_M
void init_timer0 (void)
{
	//Compare-hit
	T0MR0 = (FOSC*3/TIMER_TEILER0)-1;
	//Interrupt and Reset on MR0
	T0MCR = TxMCR_MR0I | TxMCR_MR0R;
	//Timer0 Enable
	T0TCR = TxTCR_COUNTER_ENABLE;
	//initialize the interrupt vector
	VICIntSelect    &= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer0);
	VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer0);
	VICVectAddr4     = (unsigned long)tc0_isr;
	VICVectPriority4 = 0x05;
	VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer0);
}
//Setup Timer1 Compare-Match Interrupt
//no prescaler timer runs at cclk = FOSC*PLL_M
void init_timer1 (void)
{
	//Compare-hit (10Khz)
	T1MR0 = 1;

	T1PR = 110;
	//Interrupt and Reset on MR0
	T1MCR = TxMCR_MR0I | TxMCR_MR0R;
	//Timer1 Enable
	T1TCR = TxTCR_COUNTER_ENABLE;
	//initialize the interrupt vector
	VICIntSelect    &= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer1);
	VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer1);
	VICVectAddr5     = (unsigned long)tc1_isr;
	VICVectPriority5 = 0x01;
	VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer1);
}

//Setup Timer1 Compare-Match Interrupt
//no prescaler timer runs at cclk = FOSC*PLL_M
void init_timer2 (void)
{
	//Compare-hit (10Khz)
	T2MR0 = (FOSC*3/TIMER_TEILER2)-1;
	//Interrupt and Reset on MR0
	T2MCR = TxMCR_MR0I | TxMCR_MR0R;
	//Timer2 Enable
	T2TCR = TxTCR_COUNTER_ENABLE;

	//initialize the interrupt vector
	VICIntSelect    &= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer2);
	VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer2);
	VICVectAddr26     = (unsigned long)tc2_isr;
	VICVectPriority26 = 0x05;
	VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer2);
}

//Setup Timer1 Compare-Match Interrupt
//no prescaler timer runs at cclk = FOSC*PLL_M
void init_timer3 (void)
{
	//Compare-hit (10Khz)
	T3MR0 = (FOSC*3/TIMER_TEILER3)-1;
	//Interrupt and Reset on MR0
	T3MCR = TxMCR_MR0I | TxMCR_MR0R;
	//Timer3 Enable
	T3TCR = TxTCR_COUNTER_ENABLE;

	//initialize the interrupt vector
	VICIntSelect    &= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer3);
	VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer3);
	VICVectAddr27     = (unsigned long)tc3_isr;
	VICVectPriority27 = 0x05;
	VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_Timer3);
}


volatile unsigned long alarm_on = 0;


void __attribute__ ((interrupt("IRQ"))) RTC_ISR (void)
{
  RTC_ILR 			|= ILR_RTCCIF;		//clear interrupt flag
  VICVectAddr 		= 0;				//Acknowledge Interrupt
}


void RTC_Init( void )
{
  alarm_on 			= 0;
  RTC_AMR 			= 0;
  RTC_CIIR 			= 0;
  RTC_CCR 			= 0;
  RTC_SEC			= 0;
  RTC_MIN			= 0;
  RTC_HOUR			= 0;
  RTC_DOM			= 1;
  RTC_DOW			= 1;
  RTC_DOY			= 1;
  RTC_MONTH			= 1;
  RTC_YEAR			= 2011;


  RTC_PREINT 		= PREINT_RTC;
  RTC_PREFRAC 		= PREFRAC_RTC;

  //VICIntSelect    	&= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_RTC);
  VICIntEnClr      	= VIC_CHAN_TO_MASKLOW(VIC_CHAN_NUM_RTC);
  VICVectAddr13     = (unsigned long)RTC_ISR;
  VICVectPriority13 = 0x0F;
  VICIntEnable    	= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_RTC);
}


void RTC_Start( void )
{
  //Start RTC counters
  RTC_CCR 			|= CCR_CLKEN;
  RTC_ILR 			= ILR_RTCCIF;
}


void RTC_Stop( void )
{
  //Stop RTC counters
  RTC_CCR 			&= ~CCR_CLKEN;
}


void RTC_CTCReset( void )
{
  //Reset CTC
  RTC_CCR |= CCR_CTCRST;

}


void RTC_Set_Time( RTCTime Time )
{
  RTC_SEC = Time.RTC_Sec;
  RTC_MIN = Time.RTC_Min;
  RTC_HOUR = Time.RTC_Hour;
  RTC_DOM = Time.RTC_Mday;
  RTC_DOW = Time.RTC_Wday;
  RTC_DOY = Time.RTC_Yday;
  RTC_MONTH = Time.RTC_Mon;
  RTC_YEAR = Time.RTC_Year;
}

void RTC_Set_Alarm( RTCTime Alarm )
{
  RTC_ALSEC = Alarm.RTC_Sec;
  RTC_ALMIN = Alarm.RTC_Min;
  RTC_ALHOUR = Alarm.RTC_Hour;
  RTC_ALDOM = Alarm.RTC_Mday;
  RTC_ALDOW = Alarm.RTC_Wday;
  RTC_ALDOY = Alarm.RTC_Yday;
  RTC_ALMON = Alarm.RTC_Mon;
  RTC_ALYEAR = Alarm.RTC_Year;

}

RTCTime RTC_Get_Time( void )
{
  RTCTime LocalTime;

  LocalTime.RTC_Sec = RTC_SEC;
  LocalTime.RTC_Min = RTC_MIN;
  LocalTime.RTC_Hour = RTC_HOUR;
  LocalTime.RTC_Mday = RTC_DOM;
  LocalTime.RTC_Wday = RTC_DOW;
  LocalTime.RTC_Yday = RTC_DOY;
  LocalTime.RTC_Mon = RTC_MONTH;
  LocalTime.RTC_Year = RTC_YEAR;
  return ( LocalTime );
}

void RTC_Set_Alarm_Mask( unsigned long AlarmMask )
{
  //Set alarm mask
  RTC_AMR = AlarmMask;
}

