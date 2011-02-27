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

/*
 *
http://www.ngdc.noaa.gov/geomagmodels/struts/IgrfWmmLocationSwitch

Something about declination. toDo: need to understand this thing
if important build fetch via Internet to get local data

Declination		Inclination	Horizontal 		Intensity		North Component	East Component	Vertical Component	Total Field
+ East - West	+ Down - Up									+ North - South	+ East - West	+ Down - Up

1/25/2011 		1° 26' 		63° 19' 		21,446.2 nT 	21,439.5 nT 	537.4 nT 		42,667.7 nT 		47,754.3 nT
Change per year 8' per year - 0' per year 	13.4 nT/year 	12.1 nT/year 	47.7 nT/year 	20.6 nT/year 		24.4 nT/year
 *
 *
 *
 *
 */








#include "math.h"
#include "nav/mm3parser.h"
#include "nav/navcomp.h"
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

float sin_acc_roll, cos_acc_roll, sin_acc_nick, cos_acc_nick;
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
		IO1_ON;		//MM3 Reset (rising edge)

		MM3_runtime.STATE = MM3_START_TRANSFER;
	return;

	case MM3_START_TRANSFER:

		//if (delayCycleCount++ > MM3_resetDelayCycles)
		//{
		setCompassBusy(1);
		setCompassStatus(0);
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
					MM3_runtime.X_axis = value;
					MM3_runtime.AXIS = MM3_Y;
				break;
				case MM3_Y:
					MM3_runtime.Y_axis = value;
					MM3_runtime.AXIS = MM3_Z;
				break;
				case MM3_Z:	//case MM3_Z:
					MM3_runtime.Z_axis = value;
					MM3_runtime.roll = (signed int)((ADC_standStill[ADC_ACCY] - ADC_runtime[ADC_ACCY])/48);
					MM3_runtime.nick = (signed int)((ADC_standStill[ADC_ACCX] - ADC_runtime[ADC_ACCX])/48);
					mm3calcHeading();

					MM3_runtime.AXIS = MM3_X;
				break;

			}
			setCompassStatus(1);
			setCompassBusy(0);
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
/*
	sysSetup.MM3.X_range = (X_Max - X_Min);
	sysSetup.MM3.Y_range = (Y_Max - Y_Min);
	sysSetup.MM3.Z_range = (z_Max - z_Min);

	sysSetup.MM3.X_off = (X_Max + X_Min) / 2;
	sysSetup.MM3.Y_off = (Y_Max + Y_Min) / 2;
	sysSetup.MM3.Z_off = (z_Max + z_Min) / 2;
	*/


	LED4_OFF;
}

/*void mm3calcHeading(void)
{
	 MM3_runtime.heading = 0 ;
	  if((MM3_runtime.X_axis == 0)&&(MM3_runtime.Y_axis < 0))
		  MM3_runtime.heading= PI/2.0;
	  if((MM3_runtime.X_axis == 0)&&(MM3_runtime.Y_axis > 0))
		  MM3_runtime.heading=3.0*PI/2.0;
	  if (MM3_runtime.X_axis < 0)
		  MM3_runtime.heading = PI - atan(MM3_runtime.Y_axis/MM3_runtime.X_axis);
	  if((MM3_runtime.X_axis > 0)&&(MM3_runtime.Y_axis < 0))
		  MM3_runtime.heading = -atan(MM3_runtime.Y_axis/MM3_runtime.X_axis);
	  if((MM3_runtime.X_axis > 0)&&(MM3_runtime.Y_axis > 0))
		  MM3_runtime.heading = 2.0*PI - atan(MM3_runtime.Y_axis/MM3_runtime.X_axis);


}
*/

float threePI = 3.0*PI/2.0;
float PIdiv = PI/2.0;
void mm3calcHeading(void)
{

	float compassHeading = 0;

	//int temp = Aktuell_az - acc_neutral.compass;
	//	//Lage-Berechnung mittels Acc-Messwerte
	//	//tilt = atan2_i(temp,AdWertAccNick*64);

	sin_acc_nick = ((sin_i(MM3_runtime.nick)));
	cos_acc_nick = ((cos_i(MM3_runtime.nick)));
	sin_acc_roll = ((sin_i(MM3_runtime.roll)));
	cos_acc_roll = ((cos_i(MM3_runtime.roll)));


	//nickgrad = -(IntegralNick / 1540);
	//rollgrad = -(IntegralRoll / 1540);

	//print_uart0(" n%d; r%d;s%d;c%d;s%d;c%d;",MM3_runtime.nick,MM3_runtime.roll,(int)(sin_acc_nick),(int)(cos_acc_nick),(int)(sin_acc_roll),(int)(cos_acc_roll));

	X_heading = 0;
	Y_heading = 0;

	signed int x_axis = MM3_runtime.X_axis - sysSetup.MM3.X_off;
	signed int y_axis = MM3_runtime.Y_axis - sysSetup.MM3.Y_off;
	signed int z_axis = MM3_runtime.Z_axis - sysSetup.MM3.Z_off;


	if( sysSetup.MM3.X_range > sysSetup.MM3.Y_range )
		y_axis = (y_axis*sysSetup.MM3.X_range)/sysSetup.MM3.Y_range; // perform gain matching
	else
		x_axis = (x_axis*sysSetup.MM3.Y_range)/sysSetup.MM3.X_range; // perform gain matching


	float angle = atan(y_axis/x_axis); // compute the angle
	angle = angle *180/PI;
	/*
	if( x_axis >=0 && y_axis >= 0 )
		compassHeading = angle; // quadrant +X, +Y (0 to 90)
	else if(x_axis < 0 && y_axis >= 0)
		compassHeading = 180 - angle; // quadrant -X, +Y (91 to 180)
	else if( x_axis < 0 && y_axis < 0 )
		compassHeading = 180 + angle; // quadrant -X, -Y (181 to 270)
	else if( x_axis >= 0 && y_axis < 0 )
		compassHeading = 360 - angle; // quadrant +X, -Y (271 to 359)
	*/

	MM3_runtime.heading = (int)angle;
	//return (int)(compassHeading);
}



/*
int getHeading(float x, float y, float z){
  float heading=0;
  if((x == 0)&&(y < 0))
    heading= PI/2.0;
  if((x == 0)&&(y > 0))
    heading=3.0*PI/2.0;
  if (x < 0)
    heading = PI - atan(y/x);
  if((x > 0)&&(y < 0))
    heading = -atan(y/x);
  if((x > 0)&&(y > 0))
    heading = 2.0*PI - atan(y/x);
  return  int(degrees(heading));
}
*/
