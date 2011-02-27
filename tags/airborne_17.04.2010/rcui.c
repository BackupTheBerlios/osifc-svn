/*
	file    rcUI.c
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

#include "rcui.h"

#include "pwmin.h"
#include "command.h"
#include "led.h"
#include "analog.h"
#include "engines.h"
#include "compass.h"
#include "settings.h"
#include "osifc.h"
#include "output.h"
#include "pidfc.h"
#include "pdfc.h"


void offFlightRcUI(void) {


	enginesOff();
	engineStatus = ENGINE_OFF;

	if((PWM_channel[PWM_THROTTLE] >= FULL_UP) && PWM_channel[PWM_G] <= FULL_RIGHT) {
		if (PWM_channel[PWM_R] <= FULL_RIGHT) {
			calib_MM3();
		}
	}

	if (PWM_channel[PWM_THROTTLE] >= FULL_UP && PWM_channel[PWM_G] >= FULL_LEFT) {
		//nick 0 roll full left select setting 1
		if (PWM_channel[PWM_N] >= -10 && PWM_channel[PWM_N] <= 10 && PWM_channel[PWM_R] >= FULL_LEFT){
			setupCache = (INCOMINGSETTINGS)setupCacheT0;
			printLoadSetting(0);
		}
		//nick full up roll full left select setting 2
		if (PWM_channel[PWM_N] >= FULL_UP && PWM_channel[PWM_R] >= FULL_LEFT){
			setupCache = (INCOMINGSETTINGS)setupCacheT1;
			printLoadSetting(1);
		}
		//nick full up roll 0 select setting 3
		if (PWM_channel[PWM_N] >= FULL_UP && PWM_channel[PWM_R] >= -10 && PWM_channel[PWM_R]< 10){
			setupCache = (INCOMINGSETTINGS)setupCacheT2;
			printLoadSetting(2);
		}
		//nick full up roll full right select setting 4
		if (PWM_channel[PWM_N] >= FULL_UP && PWM_channel[PWM_R] <= FULL_RIGHT){
			setupCache = (INCOMINGSETTINGS)setupCacheT3;
			printLoadSetting(3);
		}
		//nick 0 roll full right select setting 5
		if (PWM_channel[PWM_N] >= -10 && PWM_channel[PWM_N] <= 10 && PWM_channel[PWM_R] <= FULL_RIGHT){
			setupCache = (INCOMINGSETTINGS)setupCacheT4;
			printLoadSetting(4);
		}
		//nick full down roll full right select setting 6
		if (PWM_channel[PWM_N] <= FULL_DOWN && PWM_channel[PWM_R] <= FULL_RIGHT){
			setupCache = (INCOMINGSETTINGS)setupCacheT5;
			printLoadSetting(5);
		}
		setToInSettings();
		ADCStandstillValues();
		ledTest();

	}

	//start Engines if right signal is received Gas down and Pitch full right
	if((PWM_channel[PWM_THROTTLE] <= FULL_DOWN) && PWM_channel[PWM_G] <= FULL_RIGHT ) //start Engine signal
	{
		//initFCRuntime();
		//resetADCmuxer();
		engineStatus = ENGINE_ON;
		LED1_ON;
	}
}



// Different inflight rcUI to make sure the code never mixes up and does some off flight commands
void inFlightRcUI(void) {

	//if Gas < -100
	//Wait for Gas to raise until then keep the Engines turning
	if (PWM_channel[PWM_THROTTLE] <= FULL_DOWN)	{
		engineWarmup();
	}
	//start the math as we got Gas signal
	if (PWM_channel[PWM_THROTTLE] >= FULL_DOWN)
	{
		switch (fcSetup.calcMode)
		{
			case PID_MODE:
				pidDriver();
			break;
			case PD_Mode:
				pdDriver();
			break;

		}
	}
	//Wait to stop Engines
	if((PWM_channel[PWM_THROTTLE] <= FULL_DOWN) && PWM_channel[PWM_G] >= FULL_LEFT)
	{
		enginesOff();
		engineStatus = ENGINE_OFF;
		initFCRuntime();
		LED1_OFF;
	}

}



