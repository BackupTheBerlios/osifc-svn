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


#include "arch/settings.h"

#include "arch/beeper.h"
#include "arch/led.h"
#include "arch/analog.h"
#include "io/pwmin.h"
#include "io/engines.h"
#include "io/output.h"
#include "math/pidfc.h"
#include "math/pdfc.h"
#include "nav/mm3parser.h"
#include "fc/osifc.h"
#include "interface/rcui.h"
#include "interface/command.h"


char rcSetting;

void offFlightRcUI(void) {


	engines_Off();
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
			rcSetting = 0;
			printLoadSetting(rcSetting);
		}
		//nick full up roll full left select setting 2
		if (PWM_channel[PWM_N] >= FULL_UP && PWM_channel[PWM_R] >= FULL_LEFT){
			setupCache = (INCOMINGSETTINGS)setupCacheT1;
			rcSetting = 1;
			printLoadSetting(rcSetting);
		}
		//nick full up roll 0 select setting 3
		if (PWM_channel[PWM_N] >= FULL_UP && PWM_channel[PWM_R] >= -10 && PWM_channel[PWM_R]< 10){
			setupCache = (INCOMINGSETTINGS)setupCacheT2;
			rcSetting = 2;
			printLoadSetting(rcSetting);
		}
		//nick full up roll full right select setting 4
		if (PWM_channel[PWM_N] >= FULL_UP && PWM_channel[PWM_R] <= FULL_RIGHT){
			setupCache = (INCOMINGSETTINGS)setupCacheT3;
			rcSetting = 3;
			printLoadSetting(rcSetting);
		}
		//nick 0 roll full right select setting 5
		if (PWM_channel[PWM_N] >= -10 && PWM_channel[PWM_N] <= 10 && PWM_channel[PWM_R] <= FULL_RIGHT){
			setupCache = (INCOMINGSETTINGS)setupCacheT4;
			rcSetting = 4;
			printLoadSetting(rcSetting);
		}
		//nick full down roll full right select setting 6
		if (PWM_channel[PWM_N] <= FULL_DOWN && PWM_channel[PWM_R] <= FULL_RIGHT){
			setupCache = (INCOMINGSETTINGS)setupCacheT5;
			rcSetting = 5;
			printLoadSetting(rcSetting);
		}
		beepCountSetting(rcSetting);
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

	//PWM_channel[PWM_THROTTLE] = 15;

	//if Gas < -100
	//Wait for Gas to raise until then keep the Engines turning
	if (PWM_channel[PWM_THROTTLE] <= FULL_DOWN)	{
		engine_Warmup();
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
		engines_Off();
		engineStatus = ENGINE_OFF;
		initFCRuntime();
		LED1_OFF;
	}

}



