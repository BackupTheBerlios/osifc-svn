/*
	file    beeper.c
	author  Werner Oswald
	(c)		Werner Oswald
	mail	osiair@osiworx.com
	date    2010-04-17

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

    2010-04-17 initial version for public release


*/

#include "beeper.h"
#include "analog.h"
#include "settings.h"

unsigned char beepStatus 	= 0;
unsigned int beepCount 		= 0;

const float Divider 		= 5.473;

void init_beeper(void)
{
	BEEPER_DIR |= BEEPER_BIT; //BEEPER-Pin as output
	BEEPER_OFF;
}

void checkUBat(void)
{
	if (beepCount++ > 300)
	{
		if ((unsigned char)(ADC_runtime[UBAT] / Divider) < fcSetup.sysLowVoltage && beepStatus == 0)
		{
			BEEPER_ON;
			beepStatus = 1;
		} else {
			BEEPER_OFF;
			beepStatus = 0;
		}
		beepCount=0;
	}
}

char setCount = 0;
void beepCountSetting(char setting)
{
	for (setCount=0;setCount < setting + 1;setCount++)
	{
		BEEPER_ON;
		for(unsigned int tmp = 0;tmp<1000000;tmp++){asm("nop");};
		BEEPER_OFF;
		for(unsigned int tmp = 0;tmp<1000000;tmp++){asm("nop");};
	}
	BEEPER_OFF;
}



