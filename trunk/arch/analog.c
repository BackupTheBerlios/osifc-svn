/*
	file    analog.c
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

#include "arch/analog.h"
#include "arch/led.h"
#include "io/i2c.h"
#include "io/output.h"
#include "arch/settings.h"


float driftFactor = 0.01;
float ADCDriftNick;
float ADCDriftRoll;
float ADCDriftPitch;
float driftReducer = 0.0005;
char ADCCount = 0;

void resetADCmuxer(void)
{
    ADC_mux[ADC_NICK] 			= 0;
    ADC_mux[ADC_ACCY] 			= 0;
    ADC_mux[ADC_ACCX] 			= 0;
    ADC_mux[ADC_ACCZ] 			= 0;
    ADC_mux[ADC_ROLL] 			= 0;
    ADC_mux[ADC_PITCH] 			= 0;
    ADC_mux[AIRPRESSURE] 		= 0;
    ADC_mux[UBAT] 				= 0;

    ADC_runtime[ADC_NICK] 			= 0;
    ADC_runtime[ADC_ACCY] 			= 0;
    ADC_runtime[ADC_ACCX] 			= 0;
    ADC_runtime[ADC_ACCZ] 			= 0;
    ADC_runtime[ADC_ROLL] 			= 0;
    ADC_runtime[ADC_PITCH] 			= 0;
    ADC_runtime[AIRPRESSURE] 		= 0;
    ADC_runtime[UBAT] 				= 0;

}



void initSensorDrift(void)
{
	ADC_drift[ADC_ACCY] 			= 0.0;
	ADC_drift[ADC_ACCX] 			= 0.0;
	ADC_drift[ADC_ACCZ] 			= 0.0;
	ADC_drift[ADC_ROLL] 			= 0.0;
	ADC_drift[ADC_PITCH] 			= 0.0;
	ADC_drift[AIRPRESSURE] 			= 0.0;
	ADC_drift[UBAT] 				= 0.0;

	ADCDriftNick = fcSetup.ADCDriftNick * driftFactor;
	ADCDriftRoll = fcSetup.ADCDriftRoll * driftFactor;
	ADCDriftPitch = fcSetup.ADCDriftPitch * driftFactor;
}

void adjustSensorDrift(void)
{
	ADC_drift[ADC_NICK] 			+= ADCDriftNick;
	ADC_drift[ADC_ROLL] 			+= ADCDriftRoll;
	ADC_drift[ADC_PITCH] 			+= ADCDriftPitch;

	ADCDriftNick 					-= ADCDriftNick*driftReducer;
	ADCDriftRoll 					-= ADCDriftRoll*driftReducer;
	ADCDriftPitch 					-= ADCDriftPitch*driftReducer;

	driftReducer					+= 0.0000002;

	if (ADC_drift[ADC_NICK] > 100)
		ADC_drift[ADC_NICK] = 100;
	if (ADC_drift[ADC_ROLL] > 100)
		ADC_drift[ADC_ROLL] = 100;
	if (ADC_drift[ADC_PITCH] > 100)
		ADC_drift[ADC_PITCH] = 100;

	if (ADCDriftNick < 0)
		ADCDriftNick = 0;
	if (ADCDriftRoll < 0)
		ADCDriftRoll = 0;
	if (ADCDriftPitch < 0)
		ADCDriftPitch = 0;
}



void ReadADC(void)
{
	if (++ADCCount <= 8) {
		ADC_mux[0] +=( AD0DR0 >> 6 ) & 0x3FF;
		ADC_mux[1] +=( AD0DR1 >> 6 ) & 0x3FF;
		ADC_mux[2] +=( AD0DR2 >> 6 ) & 0x3FF;
		ADC_mux[3] +=( AD0DR3 >> 6 ) & 0x3FF;
		ADC_mux[4] +=( AD0DR4 >> 6 ) & 0x3FF;
		ADC_mux[5] +=( AD0DR5 >> 6 ) & 0x3FF;
		ADC_mux[6] +=( AD0DR6 >> 6 ) & 0x3FF;
		ADC_mux[7] +=( AD0DR7 >> 6 ) & 0x3FF;

	} else {
		ADC_runtime[0] = (int)ADC_mux[0] / 8;
		ADC_runtime[1] = (int)ADC_mux[1] / 8;
		ADC_runtime[2] = (int)ADC_mux[2] / 8;
		ADC_runtime[3] = (int)ADC_mux[3] / 8;
		ADC_runtime[4] = (int)ADC_mux[4] / 8;
		ADC_runtime[5] = (int)ADC_mux[5] / 8;
		ADC_runtime[6] = (int)ADC_mux[6] / 8;
		ADC_runtime[7] = (int)ADC_mux[7] / 8;

		ADC_mux[0] = 0;
		ADC_mux[1] = 0;
		ADC_mux[2] = 0;
		ADC_mux[3] = 0;
		ADC_mux[4] = 0;
		ADC_mux[5] = 0;
		ADC_mux[6] = 0;
		ADC_mux[7] = 0;

		ADCCount = 0;


	}
}


void ADCGyroSetup(int ADCChannel)
{
	int ADCTemp;
	int tmp;
	unsigned ADCRounds;

	ADCTemp = (int)ADC_runtime[ADCChannel];

	while (ADCTemp < 510)
	{
		ADC_offset[ADCChannel]--;
		I2C1State = 0;
		I2C1Mode = 0;
		I2C1_Start();
		for(ADCRounds = 0;ADCRounds<50;ADCRounds++){
			for(tmp = 0;tmp<1000;tmp++){asm("nop");};
			ReadADC();
		};
		ADCTemp = (int)ADC_runtime[ADCChannel];
		if ((int)ADC_offset[ADCChannel] == 0)
		{
			ADC_offset[ADCChannel] = 0;
			ADCTemp = 520;
		}
	}
	ADCTemp = (int)ADC_runtime[ADCChannel];
	while (ADCTemp > 510)
	{
		ADC_offset[ADCChannel]++;
		I2C1State = 0;
		I2C1Mode = 0;
		I2C1_Start();
		for(ADCRounds = 0;ADCRounds<50;ADCRounds++){
			for(tmp = 0;tmp<1000;tmp++){asm("nop");};
			ReadADC();
		};
		ADCTemp = (int)ADC_runtime[ADCChannel];
		if ((int)ADC_offset[ADCChannel] == 255)
		{
			ADC_offset[ADCChannel] = 0;
			ADCTemp = 499;
		}

	}
	for(tmp = 0;tmp<10000;tmp++){asm("nop");};
	I2C1_Stop();
}


void ADCPressureSetup(int ADCChannel)
{
	int ADCTemp;
	int tmp;
	unsigned ADCRounds;

	ADCTemp = (int)ADC_runtime[ADCChannel];
	print_uart0("FCm0;ADC start %d = %d;00#",ADCChannel,ADC_runtime[ADCChannel]);

	ADC_offset[ADCChannel] = 200;
	I2C1State = 0;
	I2C1Mode = 0;
	I2C1_Start();


	while (ADCTemp < 1000)
	{
		ADC_offset[ADCChannel]--;
		I2C1State = 0;
		I2C1Mode = 0;
		I2C1_Start();
		for(ADCRounds = 0;ADCRounds<50;ADCRounds++){
			for(tmp = 0;tmp<10000;tmp++){asm("nop");};
			ReadADC();
		};
		ADCTemp = (int)ADC_runtime[ADCChannel];
		if ((int)ADC_offset[ADCChannel] == 0)
		{
			ADC_offset[ADCChannel] = 0;
			ADCTemp = 1010;
		}
	}
	print_uart0("FCm0;ADC offset Hi %d = %d value %d;00#",ADCChannel,ADC_offset[ADCChannel],ADCTemp);
	ADCTemp = (int)ADC_runtime[ADCChannel];
	while (ADCTemp > 1000)
	{
		ADC_offset[ADCChannel]++;
		I2C1State = 0;
		I2C1Mode = 0;
		I2C1_Start();
		for(ADCRounds = 0;ADCRounds<50;ADCRounds++){
			for(tmp = 0;tmp<10000;tmp++){asm("nop");};
			ReadADC();
		};
		ADCTemp = (int)ADC_runtime[ADCChannel];
		if ((int)ADC_offset[ADCChannel] == 255)
		{
			ADC_offset[ADCChannel] = 0;
			ADCTemp = 499;
		}
	}

	print_uart0("FCm0;ADC offset Low %d = %d value %d;00#",ADCChannel,ADC_offset[ADCChannel],ADCTemp);
	for(tmp = 0;tmp<100000;tmp++){asm("nop");};
	I2C1_Stop();
}

void GyroInit(void)
{
	unsigned ADCRounds;
	int tmp;
	for(ADCRounds = 0;ADCRounds<50;ADCRounds++){
		ReadADC();
		for(tmp = 0;tmp<1000;tmp++){asm("nop");};
	};

	if (fcSetup.ADCModeRoll == 1) {
		ADCGyroSetup(ADC_ROLL);
	}
	if (fcSetup.ADCModePitch == 1) {
		ADCGyroSetup(ADC_PITCH);
	}
	if (fcSetup.ADCModeNick == 1) {
		ADCGyroSetup(ADC_NICK);
	}

}

void ADCStandstillValues(void)
{
	unsigned int tmp;
	LED3_ON;

    resetADCmuxer();
	for(tmp = 0;tmp<1000000;tmp++){asm("nop");};

	unsigned ADCRounds;
	for(ADCRounds = 0;ADCRounds<100;ADCRounds++){
		ReadADC();
		for(tmp = 0;tmp<10000;tmp++){asm("nop");};
	};
	ADC_standStill[ADC_PITCH]   = (int)(ADC_runtime[ADC_PITCH] + fcSetup.PitchSensorBias);
	ADC_standStill[ADC_NICK]    = (int)ADC_runtime[ADC_NICK];
	ADC_standStill[ADC_ROLL]    = (int)ADC_runtime[ADC_ROLL];
	ADC_standStill[ADC_ACCX]    = (int)(ADC_runtime[ADC_ACCX] + fcSetup.X_sensorBias);
	ADC_standStill[ADC_ACCY]    = (int)(ADC_runtime[ADC_ACCY] + fcSetup.Y_sensorBias);
	//ADC_standStill[ADC_ACCZ]    = (int)ADC_runtime[ADC_ACCZ];
	ADC_standStill[AIRPRESSURE] = (int)ADC_runtime[AIRPRESSURE];
	LED3_OFF;
	initSensorDrift();
}



