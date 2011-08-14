/*
	file    pdFC.c
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
#include "arch/analog.h"
#include "io/output.h"
#include "io/engines.h"
#include "math/pdfc.h"
#include "math/pidfc.h"



void initPdFC (void)
{
	enginesOff();
}


int printCounter;
void pdDriver (void)
{

	ADC_OFF;


	Pitch_Difference = ADC_standStill[ADC_PITCH] - (signed short int)(ADC_mux[ADC_PITCH]/ADC_count[ADC_PITCH]) ;

    Y_Difference_AccY =  ADC_standStill[ADC_ACCY] - (signed short int)(ADC_mux[ADC_ACCY]/ADC_count[ADC_ACCY]);
    Y_Difference_GyrN =  ADC_standStill[ADC_NICK] - (signed short int)(ADC_mux[ADC_NICK]/ADC_count[ADC_NICK]);

    X_Difference_AccX =  ADC_standStill[ADC_ACCX] - (signed short int)(ADC_mux[ADC_ACCX]/ADC_count[ADC_ACCX]);
    X_Difference_GyrR =  ADC_standStill[ADC_ROLL] - (signed short int)(ADC_mux[ADC_ROLL]/ADC_count[ADC_ROLL]);


    ADC_ON;

    Y_Response = 0, Y_Gyr_ACC_Difference = 0;
    X_Response = 0, X_Gyr_ACC_Difference = 0;
    Difference = 0, X_DifferenceFact = 0;




	Difference = (Pitch_Difference * fcSetup.pd_GyroPitchFact) + (PWM_channel[PWM_G] * fcSetup.pd_GyroStickFact);


	DifferencePitchGyr=Difference; //debug Info Only

	Pitch_Difference = (fcSetup.pd_PitchP / 100) * Difference;


	Pitch_Difference += (fcSetup.pd_PitchD / 100) * (Difference - Pitch_Difference_old);
	Pitch_Difference_old = Difference;


	X_DifferenceTotal = X_Difference_GyrR;

	X_Gyr_ACC_Difference = X_DifferenceTotal - (X_Difference_AccX * fcSetup.pd_X_AccX_Fact);

	Difference  = (X_Gyr_ACC_Difference * fcSetup.pd_X_sensorFact);
	Difference *= (PWM_channel[PWM_R] * fcSetup.pd_GyroStickFact);



	X_Response = fcSetup.pd_X_P * Difference ;



    X_Response += fcSetup.pd_X_D * (Difference - X_Difference_old);
    X_Response -= Pitch_Difference,

    X_Difference_old = Difference;




    int throttle = PWM_channel[PWM_THROTTLE] + fcSetup.pd_throttleOffset;


	Y_DifferenceTotal = Y_Difference_GyrN;

	Y_Gyr_ACC_Difference = Y_DifferenceTotal + (Y_Difference_AccY * fcSetup.pd_Y_AccY_Fact);

	Difference = (Y_Gyr_ACC_Difference * fcSetup.pd_Y_sensorFact);
	Difference *= (PWM_channel[PWM_N] * fcSetup.pd_GyroStickFact);

	Y_Response = fcSetup.pd_Y_P * Difference ;



    Y_Response += fcSetup.pd_Y_D * (Difference - Y_Difference_old);
    Y_Response += Pitch_Difference,
    Y_Difference_old = Difference;

	Engine_value = (signed int)(throttle + X_Response);
	setEngine(RIGHTENGINE,Engine_value);
	Engine_value = (signed int)(throttle - X_Response);
	setEngine(LEFTENGINE,Engine_value);
	Engine_value = (signed int)(throttle  - Y_Response);
	setEngine(FRONTENGINE,Engine_value);
	Engine_value = (signed int)(throttle  + Y_Response);
	setEngine(BACKENGINE,Engine_value);


}
