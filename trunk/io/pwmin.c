/*
	file    pwmin.c
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



#include "arch/settings.h"
#include "arch/lpc23xx.h"
#include "arch/sys_config.h"
#include "arch/clock-arch.h"
#include "arch/printf_p.h"
#include "arch/led.h"
#include "io/compass.h"
#include "io/i2c.h"
#include "io/pwmin.h"
#include "nav/mm3parser.h"

volatile unsigned long int PWMChannelR1;
volatile unsigned long int PWMChannelR2;
volatile unsigned long int PWMChannelF;
volatile unsigned long int PWMChannelF2;
volatile unsigned long int IO_PINS;
volatile unsigned long int IO_PINS2;
volatile unsigned int timerTemp;

volatile long int PWMtmp;

void PWMIN_Init_Vars(void)
{
	PWM_chan_count 		= 0;
	PWM_valid			= 0;
	PWM_channel[0] 		= 0;
	PWM_channel[1] 		= 0;
	PWM_channel[2] 		= 0;
	PWM_channel[3] 		= 0;
	PWM_channel[4] 		= 0;
	PWM_channel[5] 		= 0;
	PWM_channel[6] 		= 0;
	PWM_channel[7] 		= 0;
	PWM_channel[8] 		= 0;
	PWM_channel[9] 		= 0;
	PWM_channel[10] 	= 0;
	PWM_channel[11] 	= 0;

	PWM_mux[0] 			= 0;
	PWM_mux[1] 			= 0;
	PWM_mux[2] 			= 0;
	PWM_mux[3] 			= 0;
	PWM_mux[4] 			= 0;
	PWM_mux[5] 			= 0;
	PWM_mux[6] 			= 0;
	PWM_mux[7] 			= 0;
	PWM_mux[8] 			= 0;
	PWM_mux[9] 			= 0;
	PWM_mux[10] 		= 0;
	PWM_mux[11] 		= 0;

	PWM_count[0] 		= 0;
	PWM_count[1] 		= 0;
	PWM_count[2] 		= 0;
	PWM_count[3] 		= 0;
	PWM_count[4] 		= 0;
	PWM_count[5] 		= 0;
	PWM_count[6] 		= 0;
	PWM_count[7] 		= 0;
	PWM_count[8] 		= 0;
	PWM_count[9] 		= 0;
	PWM_count[10] 		= 0;
	PWM_count[11] 		= 0;

	OldTimerVal 		= 0;
	TimerVal 			= 0;
}

void __attribute__ ((interrupt("IRQ"))) PWMIN_MC_ISR(void)
{
	PWMtmp = IO2_INT_STAT_R;
//led_switch(2);


	if  (PWMtmp == DRDY_PIN)
	{
		//print_uart0(" PWMtemp %d ",PWMtmp);
		//for now to use it as the DRDY for the MM3 Compass
		setMM3State(MM3_DRDY);
		IO2_INT_CLR ^= PWMtmp;
		VICVectAddr = 0;						//Acknowledge Interrupt (rough?)
		return;
	}

	if  (PWMtmp == PWMIN_PIN_9)
	{
		TimerVal = timer_counter1 - OldTimerVal;
		OldTimerVal = timer_counter1;

		if (TimerVal < sysSetup.PWM.MinValue){
				PWM_chan_count = 0;
				PWM_valid = 0;
				IO2_INT_CLR ^= PWMtmp; 					//Clear GPIO IRQ
				VICVectAddr = 0;						//Acknowledge Interrupt (rough?)
				return;
			}

		if (TimerVal > sysSetup.PWM.MaxValue){
			PWM_chan_count = 0;
			PWM_valid +=1000;
		} else {
			if(PWM_valid > 0){

				PWM_mux[PWM_chan_count] += (TimerVal - sysSetup.PWM.mcOffset);
				if (PWM_chan_count > 0)
					PWM_mux[PWM_chan_count] -= sysSetup.PWM.mcOffset2;

				if (PWM_count[PWM_chan_count]++ == 1) {
					PWM_channel[PWM_chan_count] = PWM_mux[PWM_chan_count] / PWM_count[PWM_chan_count];
					PWM_mux[PWM_chan_count] = 0;
					PWM_count[PWM_chan_count] = 0;
				}

				if (PWM_channel[PWM_chan_count] > 120) PWM_channel[PWM_chan_count] = 120;
				if (PWM_channel[PWM_chan_count] < -120) PWM_channel[PWM_chan_count] = -120;
				PWM_valid += 1000;
				if(PWM_chan_count++ > sysSetup.PWM.MaxMultichannel){
					PWM_valid-=1000;
					PWM_chan_count=0;
				}
			}
		}
	}
	if (PWM_valid>10000)
		PWM_valid = 10000;
	IO2_INT_CLR ^= PWMtmp;
	VICVectAddr = 0;
	return;
}



// old version of single channel PWM in
void __attribute__ ((interrupt("IRQ"))) PWMIN_SC_ISR(void)
{

	if (PWMChannelR2 == 0x4) {
		// read the HMC compass as DRDY went high
		IO2_INT_CLR |= 0x4;
		I2C1_Start();
	} else {


		if (timer_counter1 < timerTemp)	{
			IO0_INT_EN_R |= 0x00078070;
			IO2_INT_EN_R |= 0x000000A4;
			timer_counter1 = 0;
			timerTemp = 0;
			IO0_INT_CLR = 0xffffffff;
			IO2_INT_CLR = 0xffffffff;
		} else {
			timerTemp = timer_counter1;
			PWMChannelR1 = IO0_INT_STAT_R;
			PWMChannelR2 = IO2_INT_STAT_R;
			PWMChannelF = IO0_INT_STAT_F;
			PWMChannelF2 = IO2_INT_STAT_F;

			IO_PINS = FIO0PIN;
			IO_PINS2 = FIO2PIN;

			if (PWMChannelR1 != 0) {
				switch (PWMChannelR1)
				{
				case PWMIN_PIN_0:
				PWM_SC_Temp[0] = timerTemp;
				break;
				case PWMIN_PIN_1:
				PWM_SC_Temp[1] = timerTemp;
				break;
				case PWMIN_PIN_2:
				PWM_SC_Temp[2] = timerTemp;
				break;
				case PWMIN_PIN_3:
				PWM_SC_Temp[3] = timerTemp;
				break;
				case PWMIN_PIN_4:
				PWM_SC_Temp[4] = timerTemp;
				break;
				case PWMIN_PIN_5:
				PWM_SC_Temp[5] = timerTemp;
				break;
				case PWMIN_PIN_6:
				PWM_SC_Temp[6] = timerTemp;
				break;
				}
				IO0_INT_EN_R ^= PWMChannelR1;
				IO0_INT_EN_F |= PWMChannelR1;
				IO0_INT_CLR |= PWMChannelR1;

			}
			if  (PWMChannelR2 != 0) {
				switch (PWMChannelR2)
				{
					case PWMIN_PIN_7:
						PWM_SC_Temp[7] = timerTemp;
					break;
					case PWMIN_PIN_8:
						MM3_runtime.STATE = MM3_DRDY;
					//PWM_SC_Temp[8] = timer_counter1;
					//disabled as PWM in for now to use it as the DRDY for the Compass
					//MM3_runtime.STATE = MM3_DRDY;
					break;
				}
				IO2_INT_EN_R ^= PWMChannelR2;
				IO2_INT_EN_F |= PWMChannelR2;
				IO2_INT_CLR |= PWMChannelR2;

			}

			if (PWMChannelF != 0) {
				switch (PWMChannelF)
				{
				case PWMIN_PIN_0:
					PWM_channel[0] = (signed int)(timerTemp - PWM_SC_Temp[0] - sysSetup.PWM.mpOffset[0]);
					if (PWM_channel[0] > 120)	PWM_channel[0] = 120;
					if (PWM_channel[0] < -120)	PWM_channel[0] = -120;
				break;
				case PWMIN_PIN_1:
					PWM_channel[1] = (signed int)(timerTemp - PWM_SC_Temp[1] - sysSetup.PWM.mpOffset[1]);
					if (PWM_channel[1] > 120)	PWM_channel[1] = 120;
					if (PWM_channel[1] < -120)	PWM_channel[1] = -120;
				break;
				case PWMIN_PIN_2:
					PWM_channel[2] = (signed int)(timerTemp - PWM_SC_Temp[2] - sysSetup.PWM.mpOffset[2]);
					if (PWM_channel[2] > 120) PWM_channel[2] = 120;
					if (PWM_channel[2] < -120) PWM_channel[2] = -120;
				break;
				case PWMIN_PIN_3:
					PWM_channel[3] = (signed int)(timerTemp - PWM_SC_Temp[3] - sysSetup.PWM.mpOffset[3]);
					if (PWM_channel[3] > 120)	PWM_channel[3] = 120;
					if (PWM_channel[3] < -120)	PWM_channel[3] = -120;
				break;
				case PWMIN_PIN_4:
					PWM_channel[4] = (signed int)(timerTemp - PWM_SC_Temp[4] - sysSetup.PWM.mpOffset[4]);
					if (PWM_channel[4] > 120)	PWM_channel[4] = 120;
					if (PWM_channel[4] < -120)	PWM_channel[4] = -120;
				break;
				case PWMIN_PIN_5:
					PWM_channel[5] = (signed int)(timerTemp - PWM_SC_Temp[5] - sysSetup.PWM.mpOffset[5]);
					if (PWM_channel[5] > 120)	PWM_channel[5] = 120;
					if (PWM_channel[5] < -120)	PWM_channel[5] = -120;
				break;
				case PWMIN_PIN_6:
					PWM_channel[6] = (signed int)(timerTemp - PWM_SC_Temp[6] - sysSetup.PWM.mpOffset[6]);
					if (PWM_channel[6] > 120)	PWM_channel[6] = 120;
					if (PWM_channel[6] < -120)	PWM_channel[6] = -120;

				break;
				}
				IO0_INT_EN_F ^= PWMChannelF;
				IO0_INT_EN_R |= PWMChannelF;
				IO0_INT_CLR |= PWMChannelF;

			}
			if (PWMChannelF2 != 0) {
				switch (PWMChannelF2)
				{
				case PWMIN_PIN_7:
					PWM_channel[7] = (signed int)(timerTemp - PWM_SC_Temp[7] - sysSetup.PWM.mpOffset[7]);
					if (PWM_channel[7] > 120)	PWM_channel[7] = 120;
					if (PWM_channel[7] < -120)	PWM_channel[7] = -120;
				break;
				case PWMIN_PIN_8:
					PWM_channel[8] =(signed int)(timerTemp - PWM_SC_Temp[8] - sysSetup.PWM.mpOffset[8]);
					if (PWM_channel[8] > 120)	PWM_channel[8] = 120;
					if (PWM_channel[8] < -120)	PWM_channel[8] = -120;
				break;
				}
				IO2_INT_EN_F ^= PWMChannelF2;
				IO2_INT_EN_R |= PWMChannelF2;
				IO2_INT_CLR |= PWMChannelF2;

			}
		}
	}
	PWM_valid = 10000;
	VICVectAddr = 0;
}




