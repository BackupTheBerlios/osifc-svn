/*
	file    main.c
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

//#include <stdlib.h>
//#include <string.h>

#include "clock-arch.h"
#include "analog.h"
#include "init.h"
#include "led.h"
#include "command.h"
#include "output.h"
#include "pwmout.h"
#include "settings.h"
#include "i2c.h"
#include "rcui.h"
#include "serialui.h"
#include "version.h"
#include "math.h"
#include "beeper.h"



char led2Status;
RTCTime current_time;

int adcRead = 0;
int HMC5843Read = 0;



//QuadCopt
int main(void)
{
	CSPR = CRP;  //set code protection to none

	CycleCount 				= 0;
	printCount 				= 0;
	refreshCount 			= 0;
	I2C0Mode 				= 1;
	int oldSec 				= 0;
	int countComponentCheck = 0;
	int countToCalc 		= 0;
	int countToI2C 			= 0;
	int servoCount 			= 0;

	init_system();



	print_uart0("FCm0;OsiFC v0.%d successful started;00#",VERSION);

	I2C0Mode = I2CMODE_FLIGHTMODE;
	I2C0State = 0;
	//I2C0Start();

	while(1){

		/*
		if (countComponentCheck++ > sysSetup.CYCLE.componentCycle) {
			//components enabled ?
			if (fcSetup.components[2] == 1) {
				if (navSol.packetStatus == 1 && PWM_channel[PWM_POTI1] > 80) {
					if (led2Status == 1) {
						LED2_OFF;
						led2Status = 0;
					} else {
						LED2_ON;
						led2Status = 1;
					}
					navSol.packetStatus = 0;
				} else {
					LED2_OFF;
					led2Status = 0;
				}

			}

			//compass enabled ?

			countComponentCheck = 0;
		}
		*/

		if (adcRead++ > adcRate) {
				ReadADC();
				adcRead = 0;

		}



		if (countToCalc++ > sysSetup.CYCLE.calcCycle) {
			countToCalc = 0;
			CycleCount++;

			if (servoCount++ > 80){
				setServo();
				servoCount = 0;
			}
			checkUBat();

			if (HMC5843Read++ > 130) {
				HMC5843Read = 0;
				//readHMC5843();
				//heading = calcHeading(HMC_runtime.X_axis,HMC_runtime.Y_axis,HMC_runtime.Z_axis);
			}


			//print telemtrie ?
			if(printCount++ > sysSetup.CYCLE.telemetrieCycle && sysSetup.CYCLE.telemetrieCycle != 0) {
				printTelemetrie();
				printCount = 0;
			}

			current_time = RTCGetTime();
			if (current_time.RTC_Sec != oldSec) {
				I2C1Mode = 0;

				//adjustSensorDrift();
				lastCycleCount = (int)CycleCount;
				CycleCount = 0;
				oldSec = current_time.RTC_Sec;
			}

			//the serial user interface always active
			serialUI();

			//engines Off so do...
			if (engineStatus == ENGINE_OFF) {
				//check for I2C commands to be send out
				//this is mainly for YGE setup
				checkI2C();
				//off flight RC UI to do setup and engines startup
				offFlightRcUI();			//the RC user interface off flight
			}
			//engines On so do...
			if (engineStatus == ENGINE_ON && PWM_valid >= 10000) {
				//check the in flight RC UI
				inFlightRcUI();				//the RC UI for in flight
			}

			I2C0Start();
		}
	}

	return (1);
}





