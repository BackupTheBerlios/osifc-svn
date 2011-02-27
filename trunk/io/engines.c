/*
	file    engines.c
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
#include "io/i2c.h"
#include "io/engines.h"
#include "interface/command.h"


//-------------------------------------------------------------------------------
//Stop Engines
void enginesOff(void)
{
	PWMEngOut[FRONTENGINE] = 0;
	PWMEngOut[RIGHTENGINE] = 0;
	PWMEngOut[BACKENGINE]  = 0;
	PWMEngOut[LEFTENGINE]  = 0;
	return;
}

//-------------------------------------------------------------------------------
//Start Engines
void engineWarmup(void)
{
	PWMEngOut[LEFTENGINE]  = fcSetup.sysGasMin;
	PWMEngOut[RIGHTENGINE] = fcSetup.sysGasMin;
	PWMEngOut[BACKENGINE]  = fcSetup.sysGasMin;
	PWMEngOut[FRONTENGINE] = fcSetup.sysGasMin;

//PWMEngOut[LEFTENGINE]  = 0;
//PWMEngOut[RIGHTENGINE] = 0;
//PWMEngOut[BACKENGINE]  = 0;
//PWMEngOut[FRONTENGINE] = 0;

	return;
}

void setEngine(int myEngine,int value) {
	//if value < min gas limit top min
	if (value < fcSetup.sysGasMin)
	{
		value = fcSetup.sysGasMin;
	}
	//if value > max gas limit top max
	if (value > fcSetup.sysGasMax)
	{
		value = fcSetup.sysGasMax;
	}
	//set the engine value
	PWMEngOut[myEngine] = (char)value;
	return;
}



