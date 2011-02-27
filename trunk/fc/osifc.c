/*
	file    osiFC.c
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




#include "osifc.h"

#include <stdlib.h>
#include <string.h>


#include "arch/settings.h"
#include "math/pidfc.h"
#include "math/pdfc.h"
#include "io/engines.h"
#include "interface/command.h"
#include "io/i2c.h"
#include "arch/analog.h"
#include "io/pwmout.h"
//-------------------------------------------------------------------------------
//
void osiFlightControl(void)
{

	switch (fcSetup.calcMode)
	{
		case PD_Mode:
			pdDriver();
		break;
		case PID_MODE:
			pidDriver();
		break;
		case KALMAN_MODE:

		break;
	}
}

void initFCRuntime (void) {

	Y_Difference_old = 0;
	Y_DifferenceTotal = 0;
	Y_Difference_Sum = 0;

	X_Difference_old = 0;
	X_DifferenceTotal = 0;
	X_Difference_Sum = 0;

	DifferencePitchFact = 0;
	Pitch_Difference_Sum = 0;
	Pitch_Difference_old = 0;

	DifferencePitchGyr = 0;
	Pitch_Difference = 0;

	Y_Response = 0;
	Y_Gyr_ACC_Difference = 0;
	Y_Difference_AccY = 0;
	Y_Difference_GyrN = 0;
	X_Response = 0;
	X_Gyr_ACC_Difference = 0;
	X_Difference_AccX = 0;
	X_Difference_GyrR = 0;
	Difference = 0;
	X_DifferenceFact = 0;
	Y_DifferenceFact = 0;
	Engine_value = 0;

	enginesOff();
	initCamServos();
	commandRetrived = 0;
	SettingsReceived = 0;

	ADC_runtime[ADC_NICK] 		= 0;
	ADC_runtime[ADC_ACCY] 		= 0;
	ADC_runtime[ADC_ACCX] 		= 0;
	ADC_runtime[ADC_ACCZ] 		= 0;
	ADC_runtime[ADC_ROLL] 		= 0;
	ADC_runtime[ADC_PITCH] 		= 0;
	ADC_runtime[AIRPRESSURE]	= 0;
	ADC_runtime[UBAT] 			= 0;


}





