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


#include "compass.h"

#include "settings.h"
#include "led.h"
#include "spi.h"
#include "io.h"
#include "analog.h"
#include "i2c.h"
#include "fcmath.h"
#include "pidfc.h"
#include "printf_p.h"
#include "math.h"

signed int value;
signed char MM3_LB;
signed char MM3_HB;

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




void check_MM3_DRDY (void)
{

	int i = 0;
	switch (MM3_runtime.STATE)
	{
	case MM3_RESET:
		IO0_OFF;	//MM3 aktiv
		IO1_ON;		//MM3 Reset rising edge

		MM3_runtime.STATE = MM3_START_TRANSFER;
	return;

	case MM3_START_TRANSFER:

		if (delayCycleCount++ > MM3_resetDelayCycles)
		{

			IO1_OFF;//set IO1 to low this should be the falling edge of the reset pulse
					//maybe there needs to be some pause...

//			SSP0CR1 |= NBIT(1);
//			SSP0CR0 |= (0<<3);			//SPI now in 8 bit mode
//			SSP0CR1 |= BIT(1);
//			initSSPI0IRQ();

			while ( !(SSP0SR & SSPSR_TNF) ){};

			if (MM3_runtime.AXIS == MM3_X) SSP0DR = (MM3_PS + MM3_X_AXIS);
			else if (MM3_runtime.AXIS == MM3_Y) SSP0DR = (MM3_PS + MM3_Y_AXIS);
			else SSP0DR = (MM3_PS + MM3_Z_AXIS);

			MM3_runtime.STATE = MM3_WAIT_DRDY; 		//wait for DRDY to go high
														//put in something else if you just like to wait
														//the timeouts given in the MM3 datasheet
			delayCycleCount = 0;
		}
	return;


	case MM3_DRDY:				//Measurement is ready we can read out the data

//		SSP0CR1 |= NBIT(1);
//		SSP0CR0 |= (1<<3);			//SPI now in 16 bit mode
//		SSP0CR1 |= BIT(1);
//		initSSPI0IRQ();

		//since we can do this there is no risk in having spikes
		//caused by interrupts during SPI

		//it should be possible to switch 8Bit to 16Bit mode. For what ever reason I did
		//not manage to get it working
		//its free to you to show me my fault ;)
		//for now we get values thats good for now, as it proves Hardware is working

		i = 0;
		while (i++ <= FIFOSIZE) {
			value = SSP0DR;		/* clear the RxFIFO */
		}
		SSP0DR = 0xFF;
		MM3_runtime.STATE = MM3_WAIT_HB;
	return;

	case MM3_WAIT_HB:
		if ( (SSP0SR & SSPSR_TFE) ) {
			MM3_HB = 0;
			MM3_HB = SSP0DR; //Clears SPIF in SSP0DR.

			SSP0DR = 0xFF;
			MM3_runtime.STATE = MM3_WAIT_LB;
		}
	return;

	case MM3_WAIT_LB:

		if ( (SSP0SR & SSPSR_TFE) ) {
			MM3_LB = SSP0DR; //Clears SPIF in SSP0DR.

			value = (signed int)((MM3_HB << 8) | MM3_LB);
			switch (MM3_runtime.AXIS)
			{
			case MM3_X:
				MM3_runtime.X_axis = value;
				MM3_runtime.AXIS = MM3_Y;
				break;
			case MM3_Y:
				MM3_runtime.Y_axis = value;
				MM3_runtime.AXIS = MM3_Z;
				break;
			default:	//case MM3_Z:
				MM3_runtime.Z_axis = value;
				MM3_runtime.AXIS = MM3_X;
			}

			IO0_ON;	//MM3 passive
			IO1_OFF;//IO1 low
			MM3_runtime.STATE = MM3_RESET;

			//SPI0Status = 0;
		}
	break;
	}
}


void calib_MM3(void)
{
	LED4_ON;
	int X_Min=0,X_Max=0,Y_Min=0,Y_Max=0,z_Min=0,z_Max=0;
	int MM3_x, MM3_y, MM3_z;
	char calib = 1;

	while (calib)
	{

       MM3_x = MM3_runtime.X_axis;
       MM3_y = MM3_runtime.Y_axis;
       MM3_z = MM3_runtime.Z_axis;

		if (MM3_x > X_Max) X_Max = MM3_x;
		else if (MM3_x < X_Min) X_Min = MM3_x;

		if (MM3_y > Y_Max) Y_Max = MM3_y;
		else if (MM3_y < Y_Min) Y_Min = MM3_y;

		if (MM3_z > z_Max) z_Max = MM3_z;
		else if (MM3_z < z_Min) z_Min = MM3_z;

		//calibration stops when taking back throttle
		if (PWM_channel[PWM_THROTTLE] < 100) calib = 0;
	}


	sysSetup.MM3.X_range = (X_Max - X_Min);
	sysSetup.MM3.Y_range = (Y_Max - Y_Min);
	sysSetup.MM3.Z_range = (z_Max - z_Min);

	sysSetup.MM3.X_off = (X_Max + X_Min) / 2;
	sysSetup.MM3.Y_off = (Y_Max + Y_Min) / 2;
	sysSetup.MM3.Z_off = (z_Max + z_Min) / 2;
	LED4_OFF;
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


int calcHeading (signed int X , signed int Y, signed int Z)
{
	signed int sin_acc_roll, cos_acc_roll, sin_acc_nick, cos_acc_nick;
	signed int tmpTilt = 0;
	signed int X_heading = 0;
	signed int Y_heading = 0;

	float compassHeading;

	tmpTilt = Y_Difference_AccY / 180;
	sin_acc_nick = sin_i(tmpTilt);
	cos_acc_nick = cos_i(tmpTilt);

	tmpTilt = X_Difference_AccX / 180;
	sin_acc_roll = sin_i(tmpTilt);
	cos_acc_roll = cos_i(tmpTilt);

	tmpTilt = 0;
	sin_acc_nick = sin_i(tmpTilt);
	cos_acc_nick = cos_i(tmpTilt);

	sin_acc_roll = sin_i(tmpTilt);
	cos_acc_roll = cos_i(tmpTilt);


	//X_heading = (signed int)(X * cos_acc_roll) + (signed int)(Y * sin_acc_roll * sin_acc_nick) - (signed int)(Z * cos_acc_roll * sin_acc_roll);
	//Y_heading = (signed int)(Y * cos_acc_roll) + (signed int)(Z * sin_acc_nick);

	//X_heading = (signed int)(X) + (signed int)(Y) - (signed int)(Z);
	//Y_heading = (signed int)(Y) + (signed int)(Z);

	/*
	if((X == 0)&&(Y < 0))
		compassHeading= (float)(PI/2.0);
	if((X == 0)&&(Y > 0))
		  compassHeading= (float)(3.0*PI/2.0);
	if (X < 0)
		  compassHeading = (float)(PI - atanf(Y/X));
	if((X > 0)&&(Y < 0))
		  compassHeading = (float)(-atanf(Y/X));
	if((X > 0)&&(Y > 0))
		  compassHeading = (float)(2.0*PI - atanf(Y/X));
	*/

	if (X > 0 && Y < 0)
	{						// 0-90

	};
	if (X < 0 && Y < 0)
	{						// 90 -180

	};
	if (X < 0 && Y > 0)
	{						// 180 - 270

	};
	if (X > 0 && Y > 0)
	{						// 270 - 360

	};

	float param = (float)((float)HMC_runtime.Y_axis/HMC_runtime.X_axis);
	float result = atanf(param);
	compassHeading = (result*180.0)/M_PI;

	if (compassHeading < 0) {
		compassHeading = 360 + compassHeading;
	}

	return (int)(compassHeading);
}
