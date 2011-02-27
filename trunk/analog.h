/*
	file    analog.h
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

#ifndef ANALOG_H
#define ANALOG_H

#include "clock-arch.h"
#include "lpc23xx.h"
#include "sys_config.h"
#include "printf_p.h"


//ADC0 Channels
#define ADC_NICK	1
#define ADC_ROLL	0
#define	ADC_PITCH	2
#define ADC_ACCX	3		//	Roll
#define ADC_ACCY	4		//	Nick
#define ADC_ACCZ	5		//	Z 1G is default. measurement is not trustable at freefall as is gets zero
#define AIRPRESSURE	6
#define UBAT		7




#define ADC_OFF  ADC_on = 0;
#define ADC_ON	 ADC_on = 1;

volatile unsigned char ADC_on;

//is mainly a debug value and can be removed later
volatile signed int ADC_raw[8];

volatile signed int ADC_mux[8];
volatile signed int ADC_count[8];
volatile signed int ADC_mit[8];
volatile float ADC_drift[8];
signed int ADC_offset[7];

signed int ADC_standStill[8];
//volatile char ADC_pos[8];

volatile signed int servoNick;
volatile signed int servoRoll;

volatile char ADC_CLK_DIV;
void ADCPressureSetup(int ADCChannel);
void ADCGyroSetup(int ADCChannel);
void GyroInit(void);
void ADC0_ISR(void);
void ADCStandstillValues(void);
void resetADCmuxer(void);
void initSensorDrift(void);
void adjustSensorDrift(void);
void ReadADC(void);

#endif //_ANALOG_H

