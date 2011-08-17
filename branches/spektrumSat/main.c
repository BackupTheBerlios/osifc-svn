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
#include "math.h"
#include "arch/settings.h"
#include "arch/clock-arch.h"
#include "arch/analog.h"
#include "arch/init.h"
#include "arch/led.h"
#include "arch/beeper.h"

#include "io/output.h"
#include "io/pwmout.h"
#include "io/i2c.h"
#include "io/uart.h"

#include "interface/command.h"
#include "interface/rcui.h"
#include "interface/serialui.h"

#include "nav/mm3parser.h"
#include "nav/osiGPS.h"
#include "nav/navcomp.h"
#include "version.h"

int maxCycle = 1000;
int minCycle = 700;

char led2Status;
RTCTime currentTime;

int adcRead = 0;
int compassRead = 0;

//QuadCopt
int main(void)
{
	CSPR = CRP;  //set code protection to none

	CycleCount 				= 0;
	printCount 				= 0;
	refreshCount 			= 0;
	I2C0Mode 				= 1;
	int oldSec 				= 0;
//	int countComponentCheck = 0;
	int countToCalc 		= 0;
//	int countToI2C 			= 0;
	int servoCount 			= 0;

	init_system();



	print_uart0("FCm0;OsiFC v0.2.%d successful started;00#",VERSION);

	I2C0Mode = I2CMODE_FLIGHTMODE;
	I2C0State = 0;
	//I2C0_Start();
	//parseMM3();

	fcSetup.components[gpsComponent] = 1;
	fcSetup.components[compassComponent] = 1;

	while(1){

		if (fcSetup.components[gpsComponent]== 1)
		checkGPSUart();

		if (adcRead++ > adcRate) {
				ReadADC();
				adcRead = 0;
		}

		if (countToCalc++ > sysSetup.CYCLE.calcCycle) {
			countToCalc = 0;
			CycleCount++;

			if (servoCount++ > 13){
				PWMOUT_set_Cam_Servos();
				servoCount = 0;
			}
			checkUBat();

			if (fcSetup.components[compassComponent] == 1) {
				if (compassRead++ > 90) {
					compassRead = 0;

					if (getCompassStatus() == 1 && getCompassBusy() == 0) {
						parseMM3();
					}

					//
					//if (NMEAdone == 1)
					//	NMEAdone = 0;
					//readHMC5843();
					//heading = calcHeading(HMC_runtime.X_axis,HMC_runtime.Y_axis,HMC_runtime.Z_axis);

				}
			}

			//print telemetrie ?
			if(printCount++ > sysSetup.CYCLE.telemetrieCycle && sysSetup.CYCLE.telemetrieCycle != 0) {
				//printTelemetrie();
				printCount = 0;
			}

			currentTime = RTC_Get_Time();
			if (currentTime.RTC_Sec != oldSec) {
				I2C1Mode = 0;
				adjustSensorDrift();
				lastCycleCount = (int)CycleCount;
				CycleCount = 0;
				oldSec = currentTime.RTC_Sec;



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

				// Calibrate Compass ?
				if(calib == 1)
				{
					calib_MM3();
				}

			}
			//engines On so do...
			if (engineStatus == ENGINE_ON && PWM_valid >= 10000) {
				//check the in flight RC UI
				inFlightRcUI();				//the RC UI for in flight
				// check if we run within speed limits and adjust if needed

				if (lastCycleCount > maxCycle)
				{
					sysSetup.CYCLE.calcCycle++;
				} else {
					if (lastCycleCount < minCycle) {
						sysSetup.CYCLE.calcCycle--;
					}
				}


			}
			//inFlightRcUI();
			I2C0_Start();
		}

	}


	return (1);
}





