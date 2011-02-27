/*
	file    pwmout.c
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


//this is for the PWM out channels
//mainly we change the settings in the PWM related registers of the LPC
//we have to use two of them for controlling the
//air pressure sensor to have a more flexible range to start with
//to do so we use the two PWM channels to manipulate a voltage going from
//VSS to the Amplifier with this we can change the output to the ADC port
//and again this changes the range we get shown from the sensor
//so we need to have a mechanism to measure the sensor and find the right PWM setting
//and then use this found setting for runtime usage as a variable

#include "arch/lpc23xx.h"
#include "arch/sys_config.h"
#include "arch/analog.h"
#include "arch/settings.h"
#include "io/pwmout.h"


unsigned int pwmMidLenght = 355;
unsigned int pwmMinLength = 190;
unsigned int pwmMaxLength = 520;
signed int rollInvers = 1;
signed int nickInvers = 1;
int PWMNick = 0;
int PWMRoll = 0;
signed int PWMChannelCounter = 0;
signed int PWMNickChannelDataMux = 0;
signed int PWMRollChannelDataMux = 0;
signed int PWMNickChannelData = 0;
signed int PWMRollChannelData = 0;
int nickMax = 0;
int rollMax = 0;
int nickMin = 0;
int rollMin = 0;
signed int nickPos = 0;
signed int rollPos = 0;
float nickForce = 0;
float rollForce = 0;
float servoTeiler = 1.375;



void PWMOUT_init_OutChannels(void)
{
	PINSEL3 |= (0<<4)	|(1<<5);  	//PWM1.1		//nick servo		P1.18
	PINSEL7 |= (1<<18)	|(1<<19); 	//PWM1.2		//roll servo		P3.25
	PINSEL7 |= (1<<20)	|(1<<21);	//PWM1.3		//no use yet		P3.26
	PINSEL4 |= (1<<6)	|(0<<7);	//PWM1.4		//no use yet		P2.3
	PINSEL4 |= (1<<8)	|(0<<9);	//PWM1.5		//no use yet		P2.4
	PINSEL3 |= (0<<20)	|(1<<21);	//PWM1.6		//no use yet		P1.26

	PWM1MCR = 0; 					// no PWM Interrupts
	PWM1CCR = 0;					// no capture events

	//PWM1TCR |= (1<<1);				// reset PWM Timer
	PWM1TCR |= (1<<0);				// enable PWM timer counter
	PWM1TCR |= (1<<3);				// select timer or PWM mode

	PWM1CTCR = 0;					// Count Control Register. The CTCR selects between Timer and Counter mode

	PWM1TC = 0xfffff000;
	PWM1PC = 0;

	PWM1PR = 300;					// PWM prescale timer counter

	PWM1MR0 = 4000;					// time for whole cycle
	PWM1MR1 = pwmMidLenght;			// value for PWM1.1
	PWM1MR2 = pwmMidLenght;			// value for PWM1.2
	PWM1MR3 = 0;					// value for PWM1.3
	PWM1MR4 = 0;					// value for PWM1.4
	PWM1MR5 = 0;					// value for PWM1.5
	PWM1MR6 = 0;					// value for PWM1.6

	//PWM1LER |= (1<<0);				// enable new data for Match 0
	//PWM1LER |= (1<<1);				// enable new data for Match 1  PWM1.1
	//PWM1LER |= (1<<2);				// enable new data for Match 2	PWM1.2
	//PWM1LER |= (0<<3);				// enable new data for Match 3  PWM1.3
	//PWM1LER |= (0<<4);				// enable new data for Match 4  PWM1.4
	//PWM1LER |= (0<<5);				// enable new data for Match 5  PWM1.5
	//PWM1LER |= (0<<6);				// enable new data for Match 6  PWM1.6
	//PWM1LER |= (0<<7);				// reserved never write 1 to this
	// should be done "at once" so its done this way
	PWM1LER |= (1<<0) |(1<<1) |(1<<2) |(0<<3) |(0<<4) |(0<<5) |(0<<6) |(0<<7);


	//PWM1PCR 0-8 stay 0 so all PWM are single edge
	//single edge means all PWM go high at the same time at start of a new cycle MR0
	//exception is match is 0 then the channel stays low at start of cycle
	PWM1PCR = 0;
	PWM1PCR |= (1<<9);				//1 to enablePWM1.1
	PWM1PCR |= (1<<10);				//1 to enablePWM1.2
	PWM1PCR |= (0<<11);				//1 to enablePWM1.3
	PWM1PCR |= (0<<12);				//1 to enablePWM1.4
	PWM1PCR |= (0<<13);				//1 to enablePWM1.5
	PWM1PCR |= (0<<15);				//1 to enablePWM1.6

	//PWM1TCR |= (0<<1);				// release reset on PWM timer
	PWM1IR = 0;

}


void PWMOUT_init_Cam_Servos(void)
{
	if(fcSetup.nickServoInvert == 1)
	{
		nickInvers = -1;
	}
	if(fcSetup.rollServoInvert == 1)
	{
		rollInvers = -1;
	}
	nickMax = pwmMinLength+fcSetup.nickServoMax*servoTeiler;
	rollMax = pwmMinLength+fcSetup.rollServoMax*servoTeiler;
	nickMin = pwmMinLength+fcSetup.nickServoMin*servoTeiler;
	rollMin = pwmMinLength+fcSetup.rollServoMin*servoTeiler;

	if(nickMax > pwmMaxLength)
	{
		nickMax = pwmMaxLength;
	}
	if(nickMin < pwmMinLength)
	{
		nickMin = pwmMinLength;
	}

	if(rollMax > pwmMaxLength)
	{
		rollMax = pwmMaxLength;
	}
	if(rollMin < pwmMinLength)
	{
		rollMin = pwmMinLength;
	}

	nickPos = (signed int)(fcSetup.nickServoPos-120)*servoTeiler;
	rollPos = (signed int)(fcSetup.rollServoPos-120)*servoTeiler;
	nickForce = (float)fcSetup.nickServoForce/100;
	rollForce = (float)fcSetup.rollServoForce/100;
}

void setPWMChannelData(void);
void setPWMChannelData(void)
{
	if (++PWMChannelCounter <= 5) {
		PWMNickChannelDataMux += PWM_channel[fcSetup.nickServoChan];
		PWMRollChannelDataMux += PWM_channel[fcSetup.rollServoChan];
	} else {
		PWMNickChannelData = (signed int)(PWMNickChannelDataMux / PWMChannelCounter);
		PWMRollChannelData = (signed int)(PWMRollChannelDataMux / PWMChannelCounter);
		PWMChannelCounter = 0;
		PWMNickChannelDataMux = 0;
		PWMRollChannelDataMux = 0;
	}
}

volatile int myservoCount = 0;
static int servoCountMax = 10;
volatile int PWMRollTemp = 0;
volatile int PWMNickTemp = 0;
volatile signed int servoNick;
volatile signed int servoRoll;

void PWMOUT_set_Cam_Servos(void)
{

	if (++myservoCount <= servoCountMax) {
		setPWMChannelData();

		servoNick = ADC_standStill[ADC_ACCY]-ADC_runtime[ADC_ACCY];
		servoRoll = ADC_standStill[ADC_ACCX]-ADC_runtime[ADC_ACCX];

		PWMNick = pwmMidLenght+(signed int)(((float)(servoNick*nickForce)+PWMNickChannelData)*servoTeiler*nickInvers);
		PWMRoll = pwmMidLenght+(signed int)(((float)(servoRoll*rollForce)+PWMRollChannelData)*servoTeiler*rollInvers);
		PWMRollTemp += (unsigned int)PWMNick+nickPos;
		PWMNickTemp += (unsigned int)PWMRoll+rollPos;

	} else {

		PWMNick = PWMRollTemp / myservoCount-1;
		PWMRoll = PWMNickTemp / myservoCount-1;

		PWMNickTemp = 0;
		PWMRollTemp = 0;
		myservoCount = 0;

		if(PWMNick > nickMax)
		{
			PWMNick = nickMax;
		}
		if(PWMNick < nickMin)
		{
			PWMNick = nickMin;
		}

		if(PWMRoll > rollMax)
		{
			PWMRoll = rollMax;
		}
		if(PWMRoll < rollMin)
		{
			PWMRoll = rollMin;
		}

		PWM1MR1 = PWMNick;		// value for PWM1.1
		PWM1MR2 = PWMRoll;		// value for PWM1.2

		PWM1LER |= (1<<1) | (1<<2);			// enable new data for Match 1&2  PWM1.1 & PWM1.2
	}

}




