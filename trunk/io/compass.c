/*
	file    compass.c
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
#include "arch/led.h"
#include "io/compass.h"
#include "io/spi.h"
#include "io/io.h"
#include "io/i2c.h"
#include "math.h"
#include "nav/mm3parser.h"




void initI2CRdWr(void);
void initI2CRdWr(void)
{
	I2CWrIndex = 0;
	I2CRdIndex = 0;
}

void setI2CRlWl(char read, char write);
void setI2CRlWl(char read, char write)
{
	I2CWriteLength = write;
	I2CReadLength = read;
}



int tmp;
void setHMC5843Continous(void)
{
	for(tmp = 0;tmp<1000000;tmp++){asm("nop");};
	HMCBuffer[0] = 0x3C;
	HMCBuffer[1] = 0x02;
	HMCBuffer[2] = 0x00;
	setI2CRlWl(0,2);
	initI2CRdWr();
	I2C1Start();
}

void setHMC5843Rate(void)
{
	for(tmp = 0;tmp<1000000;tmp++){asm("nop");};
	HMCBuffer[0] = 0x3C;
	HMCBuffer[1] = 0x00;
	HMCBuffer[2] = 0x4;
	setI2CRlWl(0,2);
	initI2CRdWr();
	I2C1Start();
}

void setHMC5843Gain(void)
{
	for(tmp = 0;tmp<1000000;tmp++){asm("nop");};
	HMCBuffer[0] = 0x3C;
	HMCBuffer[1] = 0x01;
	HMCBuffer[2] = 0x07;
	setI2CRlWl(0,2);
	initI2CRdWr();
	I2C1Start();
}



void initialreadHMC5843(void)
{
	for(tmp = 0;tmp<1000000;tmp++){asm("nop");};
	HMCBuffer[0] = 0x3D;
	setI2CRlWl(6,0);
	initI2CRdWr();
	I2C1Start();
}

void readHMC5843(void)
{
	HMCBuffer[0] = 0x3D;
	setI2CRlWl(6,0);
	initI2CRdWr();
	I2C1Start();
	led_switch(1);
}




void calcHMC5843Offset(void)
{
	if (HMC_runtime.X_axis < HMC_runtime.X_axis_min)
	{
		HMC_runtime.X_axis_min = HMC_runtime.X_axis;
	}
	if (HMC_runtime.X_axis > HMC_runtime.X_axis_max)
	{
		HMC_runtime.X_axis_max = HMC_runtime.X_axis;
	}
	HMC_runtime.X_axis_range = HMC_runtime.X_axis_max + HMC_runtime.X_axis_min;

	if (HMC_runtime.Y_axis < HMC_runtime.Y_axis_min)
	{
		HMC_runtime.Y_axis_min = HMC_runtime.Y_axis;
	}
	if (HMC_runtime.Y_axis > HMC_runtime.Y_axis_max)
	{
		HMC_runtime.Y_axis_max = HMC_runtime.Y_axis;
	}
	HMC_runtime.Y_axis_range = HMC_runtime.Y_axis_max + HMC_runtime.Y_axis_min;

	if (HMC_runtime.Z_axis < HMC_runtime.Z_axis_min)
	{
		HMC_runtime.Z_axis_min = HMC_runtime.Z_axis;
	}
	if (HMC_runtime.Z_axis > HMC_runtime.Z_axis_max)
	{
		HMC_runtime.Z_axis_max = HMC_runtime.Z_axis;
	}
	HMC_runtime.Z_axis_range = HMC_runtime.Z_axis_max + HMC_runtime.Z_axis_min;
}


void initHMC6343(void)
{
	HMCBuffer[0] = 0x32;
	HMCBuffer[1] = 0x50;
	setI2CRlWl(0,1);
	initI2CRdWr();
	led_switch(3);
	I2C1Start();
}

void readHMC6343(void)
{
	HMCBuffer[0] = 0x33;
	setI2CRlWl(6,0);
	initI2CRdWr();
	I2C1Start();
	led_switch(2);
}


