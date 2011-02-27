/*
	file    mm3parser.c
	author  Werner Oswald
	(c)		Werner Oswald
	mail	osiair@osiworx.com
	date    11.01.2011

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

    11.01.2011 initial version for public release



*/
#include "math.h"
#include "nav/mm3parser.h"

#include "arch/settings.h"
#include "arch/analog.h"
#include "arch/led.h"
#include "io/io.h"
#include "io/spi.h"
#include "io/pwmin.h"

#include "math/fcmath.h"

signed int value;
signed char MM3_LB;
signed char MM3_HB;
int delayCycleCount = 0;

signed int sin_acc_roll, cos_acc_roll, sin_acc_nick, cos_acc_nick;
signed int tmpTilt = 0;
signed int  X_heading = 0;
signed int  Y_heading = 0;

void setMM3State(char state)
{
	MM3_runtime.STATE = state;
}

void parseMM3 (void)
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

		//if (delayCycleCount++ > MM3_resetDelayCycles)
		//{

			IO1_OFF;//set IO1 to low this should be the falling edge of the reset pulse
					//maybe there needs to be some pause...

//			SSP0CR1 |= NBIT(1);
//			SSP0CR0 |= (0<<3);			//SPI now in 8 bit mode
//			SSP0CR1 |= BIT(1);
//			initSSPI0IRQ();

			while ( !(SSP0SR & SSPSR_TNF) ){};

			switch (MM3_runtime.AXIS) {
				case MM3_X:

					SSP0DR = (MM3_PS + MM3_X_AXIS);
				return;
				case MM3_Y:
					SSP0DR = (MM3_PS + MM3_Y_AXIS);
				return;
				case MM3_Z:
					SSP0DR = (MM3_PS + MM3_Z_AXIS);
				return;
			}

			MM3_runtime.STATE = MM3_WAIT_DRDY; 		//wait for DRDY to go high
													//put in something else if you just like to wait
													//the timeouts given in the MM3 datasheet
		//	delayCycleCount = 0;
		//}
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
		//its up to you to show me my fault ;)

		led_switch(3);
		i = 0;
		while (i++ <= FIFOSIZE) {
			value = SSP0DR;		/* clear the RxFIFO */
		}
		SSP0DR = 0xFF;
		MM3_runtime.STATE = MM3_WAIT_HB;
	return;

	case MM3_WAIT_HB:
		if ( (SSP0SR & SSPSR_TFE) ) {
			//MM3_HB = 0;
			MM3_HB = SSP0DR; //Clears SPIF in SSP0DR.
			SSP0DR = 0xFF;
			MM3_runtime.STATE = MM3_WAIT_LB;
		}
	return;

	case MM3_WAIT_LB:
		//if ( (SSP0SR & SSPSR_TFE) ) {
			MM3_LB = SSP0DR; //Clears SPIF in SSP0DR.
			value = (signed int)((MM3_HB << 8) | MM3_LB);
			switch (MM3_runtime.AXIS)
			{
				case MM3_X:
					MM3_runtime.X_axis = value;//-sysSetup.MM3.X_off;
					MM3_runtime.AXIS = MM3_Y;
				break;
				case MM3_Y:
					MM3_runtime.Y_axis = value;//-sysSetup.MM3.Y_off;
					MM3_runtime.AXIS = MM3_Z;
				break;
				case MM3_Z:	//case MM3_Z:
					MM3_runtime.Z_axis = value;//-sysSetup.MM3.Z_off;
					MM3_runtime.roll = (signed int)((ADC_standStill[ADC_ACCY] - ADC_runtime[ADC_ACCY]));
					MM3_runtime.nick = (signed int)((ADC_standStill[ADC_ACCX] - ADC_runtime[ADC_ACCX]));
					mm3calcHeading();
					MM3_runtime.AXIS = MM3_X;
				break;

			}
			IO0_ON;	//MM3 passive
			IO1_OFF;//IO1 low
			MM3_runtime.STATE = MM3_RESET;
			//SPI0Status = 0;
		//}
	return;
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

		if (MM3_x > X_Max)
			X_Max = MM3_x;
		else
			if (MM3_x < X_Min)
				X_Min = MM3_x;

		if (MM3_y > Y_Max)
			Y_Max = MM3_y;
		else
			if (MM3_y < Y_Min)
				Y_Min = MM3_y;

		if (MM3_z > z_Max)
			z_Max = MM3_z;
		else
			if (MM3_z < z_Min)
				z_Min = MM3_z;

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


void mm3calcHeading(void)
{

	signed int compassHeading = 0;

	sin_acc_nick = ((sin_i(MM3_runtime.nick)));
	cos_acc_nick = ((cos_i(MM3_runtime.nick)));
	sin_acc_roll = ((sin_i(MM3_runtime.roll)));
	cos_acc_roll = ((cos_i(MM3_runtime.roll)));

	//print_uart0(" nick%d; roll%d;sin%d;cos%d;  ",MM3_runtime.nick,MM3_runtime.roll,(int)(sin_acc_nick),(int)(cos_acc_nick));

	X_heading = 	  (MM3_runtime.X_axis * cos_acc_nick/1000)
					- (((MM3_runtime.Y_axis * sin_acc_roll/1000) 	- (MM3_runtime.Z_axis * cos_acc_roll/1000)) * sin_acc_nick/1000)
					;

	Y_heading = 	  (MM3_runtime.Y_axis * cos_acc_roll/1000)
					+ (MM3_runtime.Z_axis * sin_acc_roll/1000)
					;

	compassHeading = atan2_i(X_heading,Y_heading); //result;//(result*180.0)/PI;

	if (compassHeading < 0) {
		compassHeading = compassHeading +180;
	}
	/* else {
		compassHeading = 360 + compassHeading;
	}*/

	if (compassHeading > 360) {
		compassHeading = 360;
	}

	//print_uart0(" X%d; Y%d; H%d;  ;00#",X_heading,Y_heading,compassHeading );

	MM3_runtime.heading = (int)compassHeading;
	//return (int)(compassHeading);
}

