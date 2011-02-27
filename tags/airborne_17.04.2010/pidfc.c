/*
	file    pidFC.c
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

#include <stdlib.h>

#include "pidfc.h"

#include "engines.h"
#include "analog.h"
#include "pwmin.h"
#include "settings.h"
#include "output.h"
//#include "float.h"

int throttle;
int accOneG = 250;
float X_AccGyroLimiter = 0.0;
float YAccGyroLimiter = 0.0;
float XStickLimiter = 0.0;
float YStickLimiter = 0.0;
int XACCDelta = 0;
int X_Gyr_ACC_Difference_old = 0;

void pidDriver (void)
{
	ADC_OFF;

	Pitch_Difference 	= ADC_standStill[ADC_PITCH]-ADC_mit[ADC_PITCH]-(int)ADC_drift[ADC_PITCH];
	Y_Difference_AccY 	= ADC_standStill[ADC_ACCY]-ADC_mit[ADC_ACCY];
    Y_Difference_GyrN 	= -1*(ADC_standStill[ADC_NICK]-ADC_mit[ADC_NICK]-(int)ADC_drift[ADC_NICK]);
    X_Difference_AccX 	= ADC_standStill[ADC_ACCX]-ADC_mit[ADC_ACCX];
//X_Difference_AccX 	= -1*(ADC_standStill[ADC_ACCX]-ADC_mit[ADC_ACCX]);
    X_Difference_GyrR 	= ADC_standStill[ADC_ROLL]-ADC_mit[ADC_ROLL]-(int)ADC_drift[ADC_ROLL];
//X_Difference_GyrR 	= -1*(ADC_standStill[ADC_ROLL]-ADC_mit[ADC_ROLL]-(int)ADC_drift[ADC_ROLL]);
    ADC_ON;


    //Pitch axis
    //the raw sensor value
    DifferencePitchGyr = Pitch_Difference;
    //limit the raw value
    DifferencePitchFact = (signed int)(Pitch_Difference * fcSetup.pid_GyroPitchFact);
    //add limited raw value to I
    Pitch_Difference_Sum += DifferencePitchFact;
    //integral > max limit to max
    if (Pitch_Difference_Sum > fcSetup.pid_Pitch_IntegralMax)
	{
    	Pitch_Difference_Sum = fcSetup.pid_Pitch_IntegralMax;
	}
    //integral < min limit to min
	if (Pitch_Difference_Sum < fcSetup.pid_Pitch_IntegralMin)
	{
		Pitch_Difference_Sum = fcSetup.pid_Pitch_IntegralMin;
	}



    //Pitch P
    Pitch_Difference = (signed int)(fcSetup.pid_PitchP  * DifferencePitchFact);
    //Pitch I
    Pitch_Difference += (signed int)(fcSetup.pid_PitchI * Pitch_Difference_Sum);
    //Pitch D
    Pitch_Difference += (signed int)(fcSetup.pid_PitchD * (DifferencePitchFact + Pitch_Difference_old));
    //Pitch Stick
    Pitch_Difference += (signed int)(fcSetup.pid_PitchStickFact * PWM_channel[PWM_G]);

    Pitch_Difference_old = DifferencePitchFact;

//DifferencePitchFact = 0;
//Pitch_Difference = 0;
    //Normalize throttle value to get -127 -> 127 to 0 to 255
    throttle = PWM_channel[PWM_THROTTLE] + fcSetup.pid_throttleOffset;

    //here we try to tackle the changes in throttle due to Pitch control
    //Subtract Pitch reaction from throttle
    signed int PitchLimiter = (Pitch_Difference * fcSetup.pid_PitchThrottleFact);

PitchLimiter = 0;

    //Roll axis
	XACCDelta = (X_Gyr_ACC_Difference_old - X_Gyr_ACC_Difference) / 50;
	X_Gyr_ACC_Difference_old = X_Gyr_ACC_Difference;
	//Normalize Gyro and ACC Signal
    X_Gyr_ACC_Difference = (signed int)(X_Difference_AccX * fcSetup.pid_X_AccX_Fact);
    // ACC limits the Gyro to help ACC to have more force when having large ACC values
    // this should help ACC force to be more reactive and not to force against the gyro
    X_AccGyroLimiter = X_Difference_GyrR + XACCDelta;
X_AccGyroLimiter = 0.0;

    X_Gyr_ACC_Difference += (signed int)(X_Difference_GyrR * (fcSetup.pid_X_GyroFact - X_AccGyroLimiter));
    //X_Gyr_ACC_Difference += (signed int)(X_Difference_GyrR - X_AccGyroLimiter * (fcSetup.pid_X_GyroFact));


    //Gyr ACC > max limit to max
    if(X_Gyr_ACC_Difference > fcSetup.pid_X_GyroACCFactMax)
	{
    	X_Gyr_ACC_Difference = fcSetup.pid_X_GyroACCFactMax;
	}
    //Gyr ACC < min limit to min
	if(X_Gyr_ACC_Difference < fcSetup.pid_X_GyroACCFactMin)
	{
		X_Gyr_ACC_Difference = fcSetup.pid_X_GyroACCFactMin;
	}


    //PID - controller
	//Limits the Gyro ACC reaction relative to what the Stick wants to achieve
	//This should help the stick has not to work against the sensors
	XStickLimiter = fcSetup.pid_X_GyroACCFact / 120 * abs(PWM_channel[PWM_R]);
XStickLimiter = 0.0;
    //Difference = X_DifferenceFact * factor from setting (expected = 0) 0 is expected as in a perfect world no change would ever happen
	X_DifferenceFact = X_Gyr_ACC_Difference * (fcSetup.pid_X_GyroACCFact - XStickLimiter);

    //calculate Integral I
    X_Difference_Sum += X_DifferenceFact;

    //integral > max limit to max integral coming from setting
    if (X_Difference_Sum > fcSetup.pid_X_IntegralMax)
	{
    	X_Difference_Sum = fcSetup.pid_X_IntegralMax;
	}

    //integral < min limit to min integral coming from setting
	if (X_Difference_Sum < fcSetup.pid_X_IntegralMin)
	{
		X_Difference_Sum = fcSetup.pid_X_IntegralMin;
	}


    //Roll P
    X_Response  = (signed int)(fcSetup.pid_X_P * X_DifferenceFact);
    //Roll I
    X_Response += (signed int)(fcSetup.pid_X_I * X_Difference_Sum);
    //Roll D
    X_Response += (signed int)(fcSetup.pid_X_D * (X_DifferenceFact + X_Difference_old));
    //Roll Stick
    X_Response -= (signed int)(fcSetup.pid_StickFact * PWM_channel[PWM_R]);


    //save old value for D
    X_Difference_old = X_DifferenceFact;


    //Nick axis

	//Normalize Gyr Signal and AAC
	Y_Gyr_ACC_Difference = (signed int)(Y_Difference_GyrN * fcSetup.pid_Y_GyroFact);
	// ACC limits the Gyro to help ACC to have more force when having large ACC values
	// this should help ACC force to be more reactive and not to force against the gyro
	YAccGyroLimiter = fcSetup.pid_Y_GyroFact / accOneG * abs(X_Difference_AccX);
YAccGyroLimiter = 0;
    Y_Gyr_ACC_Difference += (signed int)(Y_Difference_AccY * (fcSetup.pid_Y_AccY_Fact - YAccGyroLimiter));

    //Gyr ACC > max limit to max
    if(Y_Gyr_ACC_Difference > fcSetup.pid_Y_GyroACCFactMax)
	{
    	Y_Gyr_ACC_Difference = fcSetup.pid_Y_GyroACCFactMax;
	}
    //Gyr ACC < min limit to min
		if(Y_Gyr_ACC_Difference < fcSetup.pid_Y_GyroACCFactMin)
		{
			Y_Gyr_ACC_Difference = fcSetup.pid_Y_GyroACCFactMin;
		}


	//Limits the Gyro ACC reaction relative to what the Stick wants to achieve
	//This should help the stick has not to work against the sensors
	YStickLimiter = fcSetup.pid_Y_GyroACCFact / 120 * abs(PWM_channel[PWM_N]);
YStickLimiter = 0;
	//Difference gets regulated to limit max reaction to it
	Y_DifferenceFact = (signed int)(Y_Gyr_ACC_Difference * (fcSetup.pid_Y_GyroACCFact - YStickLimiter));

	//calculate Integral
	Y_Difference_Sum += Y_DifferenceFact;
	//integral > max limit to max
	if (Y_Difference_Sum > fcSetup.pid_Y_IntegralMax)
	{
		Y_Difference_Sum = fcSetup.pid_Y_IntegralMax;
	}
	//integral < min limit to min
		if (Y_Difference_Sum < fcSetup.pid_Y_IntegralMin)
		{
			Y_Difference_Sum = fcSetup.pid_Y_IntegralMin;
		}

	//do the controlling PID math
	//Nick P
	Y_Response  = (signed int)(fcSetup.pid_Y_P * Y_DifferenceFact);
	//Nick I
	Y_Response += (signed int)(fcSetup.pid_Y_I * Y_Difference_Sum);
	//Nick D
	Y_Response += (signed int)(fcSetup.pid_Y_D * (Y_DifferenceFact + Y_Difference_old));
	//Nick Stick
	Y_Response -= (signed int)(fcSetup.pid_StickFact * PWM_channel[PWM_N]);


	//save old value for D
	Y_Difference_old = Y_DifferenceFact;


	//set the engine values
	int pitchFact = (int)(fcSetup.pid_X_PitchFact * Pitch_Difference + PitchLimiter);
	Engine_value = (int)((throttle - X_Response) - pitchFact);
	setEngine(RIGHTENGINE,Engine_value);
	Engine_value = (int)((throttle + X_Response) - pitchFact);
	setEngine(LEFTENGINE,Engine_value);
	//this is for single axis testing
	//PWMEngOut[RIGHTENGINE] = 0;
	//PWMEngOut[LEFTENGINE] = 0;


	pitchFact = (int)(fcSetup.pid_Y_PitchFact * Pitch_Difference)- PitchLimiter;
	Engine_value = (int)((throttle + Y_Response) + pitchFact);
	setEngine(FRONTENGINE,Engine_value);
	Engine_value = (int)((throttle - Y_Response) + pitchFact);
	setEngine(BACKENGINE,Engine_value);
	//this is for single axis testing
	//PWMEngOut[FRONTENGINE] = 0;
	//PWMEngOut[BACKENGINE] = 0;
}


