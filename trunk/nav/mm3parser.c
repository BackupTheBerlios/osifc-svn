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

#include "mm3parser.h"
#include "math.h"
#include "stdlib.h"
#include "navcomp.h"
#include "../arch/settings.h"
#include "../arch/analog.h"
#include "../arch/led.h"
#include "../io/io.h"
#include "../io/spi.h"
#include "../io/pwmin.h"
#include "../math/fcmath.h"



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
	return;
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

		//led_switch(3);
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
					MM3_runtime.X_axis = value - sysSetup.MM3.X_off;
					MM3_runtime.AXIS = MM3_Y;
				break;
				case MM3_Y:
					MM3_runtime.Y_axis = value - sysSetup.MM3.Y_off;
					MM3_runtime.AXIS = MM3_Z;
				break;
				case MM3_Z:	//case MM3_Z:
					MM3_runtime.Z_axis = value - sysSetup.MM3.Z_off;
					MM3_runtime.roll = (signed int)(ADC_standStill[ADC_ACCY] - ADC_runtime[ADC_ACCY]);
					MM3_runtime.nick = (signed int)(ADC_standStill[ADC_ACCX] - ADC_runtime[ADC_ACCX]);
					MM3_runtime.ground = (signed int)(ADC_standStill[ADC_ACCZ] - ADC_runtime[ADC_ACCZ]);
					MM3_runtime.roll = ADC_runtime[ADC_ACCY];
					MM3_runtime.nick = ADC_runtime[ADC_ACCX];
					MM3_runtime.ground = ADC_runtime[ADC_ACCZ];

					print_uart0("MM3 %d;%d;%d;%d;%d;%d ",
							(signed int)MM3_runtime.X_axis,
							(signed int)MM3_runtime.Y_axis,
							(signed int)MM3_runtime.Z_axis,
							(signed int)MM3_runtime.roll,
							(signed int)MM3_runtime.nick,
							(signed int)MM3_runtime.ground

					);
					print_uart0(";00#");
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


/*	will make LED 1-3 blink until no new Data gets read.
 * 	new data will be a new value < or > the already known min and max values
 */

volatile signed int X_Min=999999,X_Max=-999999,Y_Min=999999,Y_Max=-999999,Z_Min=999999,Z_Max=-999999;
signed int MM3_x, MM3_y, MM3_z;

void calib_MM3(void)
{

	led_switch(1);

	if (MM3_x != MM3_runtime.X_axis) {
		if (MM3_runtime.X_axis > X_Max){
			X_Max = MM3_runtime.X_axis;
			led_switch(2);
		}else {
			if (MM3_runtime.X_axis < X_Min){
				X_Min = MM3_runtime.X_axis;
				led_switch(2);
			} else {
				LED2_OFF;
			}
		}
	}

	if (MM3_y != MM3_runtime.Y_axis) {
		if (MM3_runtime.Y_axis > Y_Max){
			Y_Max = MM3_runtime.Y_axis;
			led_switch(3);
		} else {
			if (MM3_runtime.Y_axis < Y_Min){
				Y_Min = MM3_runtime.Y_axis;
				led_switch(3);
			} else {
				LED3_OFF;
			}
		}
	}


	if (MM3_z != MM3_runtime.Z_axis) {
		if (MM3_runtime.Z_axis > Z_Max){
			Z_Max = MM3_runtime.Z_axis;
			led_switch(4);
		} else {
			if (MM3_runtime.Z_axis < Z_Min){
				Z_Min = MM3_runtime.Z_axis;
				led_switch(4);
			} else {
				LED4_OFF;
			}
		}
	}


	//calibration stops when taking back throttle
	//if (PWM_channel[PWM_THROTTLE] < 100) calib = 0;


	// Calculate the values. Done every time which is a waste of resources.
	// But we do it in off flight and setup situation anyways.

	// Calculate the Compass range value

	sysSetup.MM3.X_MAX = X_Max;
	sysSetup.MM3.Y_MAX = Y_Max;
	sysSetup.MM3.Z_MAX = Z_Max;
	sysSetup.MM3.X_MIN = X_Min;
	sysSetup.MM3.Y_MIN = Y_Min;
	sysSetup.MM3.Z_MIN = Z_Min;

	sysSetup.MM3.X_range = (X_Max - X_Min);
	sysSetup.MM3.Y_range = (Y_Max - Y_Min);
	sysSetup.MM3.Z_range = (Z_Max - Z_Min);

	// Calculate the Compass gain offset value

	sysSetup.MM3.X_off = (X_Max + X_Min) / 2;
	sysSetup.MM3.Y_off = (Y_Max + Y_Min) / 2;
	sysSetup.MM3.Z_off = (Z_Max + Z_Min) / 2;


}


//float threePI = 3.0*PI/2.0;
//float PIdiv = PI/2.0;

// Tilt compensated MM3

void mm3calcHeading(void)
{

	float sin_nick, cos_nick, sin_roll, cos_roll;
	volatile signed int X_corr, Y_corr;
	volatile signed int angle;
	//float nickgrad,rollgrad,nick,roll;

	/*nickgrad = (float)((float)MM3_runtime.nick / 1430); //Teiler ermittelt durch Test bestätigung durch Holger
	rollgrad = (float)((float)MM3_runtime.roll / 1430); //Teiler ermittelt durch Test bestätigung durch Holger
	nick = (float)(nickgrad/180 * M_PI);// * (float)winkelkorr[lookup_nick]*(float);
	roll = (float)(rollgrad/180 * M_PI);// * (float)winkelkorr[lookup_roll]*(float);



	print_uart0("nickRoll %d;%d;%d;%d",
					(signed int)(nickgrad * 1000),
					(signed int)(rollgrad * 1000),
					(signed int)(nick *100000),
					(signed int)(roll*100000)

				);
			print_uart0(";00#");
*/

	// Berechung von sinus und cosinus
	sin_nick = sin(MM3_runtime.nick);
	cos_nick = cos(MM3_runtime.nick);
	sin_roll = sin(MM3_runtime.roll);
	cos_roll = cos(MM3_runtime.roll);



	//Kompensationsroutine
	//XE = (double)Data_axe[0] * (double)cos(nick) - Data_axe[1] * (double)sin(nick) * (double)sin(roll) - Data_axe[2] * (double)sin(nick) * (double)cos(roll);

	//YE = (double)Data_axe[1] * (double)cos(roll) + Data_axe[2] * (double)sin(roll);

	//Kompensationsroutine
	X_corr = (signed int)((float)MM3_runtime.X_axis * (float)cos_nick - (float)MM3_runtime.Y_axis * (float)sin_nick * (float)sin_roll -  (float)MM3_runtime.Z_axis * (float)sin_nick * (float)cos_roll);

	Y_corr = (signed int)((float)MM3_runtime.Y_axis * (float)cos_roll +  (float)MM3_runtime.Z_axis * (float)sin_roll);



	// Neigungskompensation
	//X_corr = (signed int)((cos_nick * MM3_runtime.X_axis) + (((sin_roll *  MM3_runtime.Y_axis) + (cos_roll * MM3_runtime.Z_axis)) * sin_nick));
	//Y_corr = (signed int)((cos_roll * MM3_runtime.Y_axis) + (sin_roll * MM3_runtime.Z_axis));





	print_uart0("1 ;00#");
	// Winkelberechnung
	angle = atan2_i(X_corr, Y_corr);
	print_uart0("headraw %d",
				/*(signed int)MM3_runtime.roll,
				(signed int)MM3_runtime.nick,*/
				(signed int)angle
			);
		print_uart0(";00#");

		if(X_corr ==0 && Y_corr <0) angle = 90;
		if(X_corr ==0 && Y_corr >0) angle = 270;
		if(X_corr < 0) angle = 180-(atan(Y_corr/X_corr)*180/M_PI);
		if(X_corr > 0 && Y_corr < 0) angle = -(atan(Y_corr/X_corr)*180/M_PI);
		if(X_corr > 0 && Y_corr > 0) angle = 360 - (atan(Y_corr/X_corr)*180/M_PI);


		print_uart0("heading %d",
					/*(signed int)X_corr,
					(signed int)Y_corr,*/
					(signed int)angle
				);
		print_uart0(";00#");

		//unsigned int ;
		float Gtot,nick,roll;

		Gtot = (float) (sqrt((MM3_runtime.nick * MM3_runtime.nick)
				+ (MM3_runtime.roll * MM3_runtime.roll)
				+ (MM3_runtime.ground * MM3_runtime.ground)));

		nick = (float) ((float)(asin(MM3_runtime.nick) / Gtot));

		roll = (float) ((float)(asin(MM3_runtime.roll) / (float)((float)(cos(MM3_runtime.nick)) * Gtot)));

		/*print_uart0("headneu %d;%d;%d",
									(signed int)(Gtot * 1000),
									(signed int)(nick * 1000),
									(signed int)(roll * 1000)
								);
				print_uart0(";00#");*/

		X_corr = (signed int)((float)(MM3_runtime.X_axis * cos(nick)) + (float)((MM3_runtime.Y_axis * sin(roll)) * sin(nick)) - (float)((MM3_runtime.Z_axis * cos(roll)) * sin(nick)));
		Y_corr = (signed int)((float)(MM3_runtime.Y_axis * cos(roll)) + (float)(MM3_runtime.Z_axis * sin(roll)));

		angle = abs(atan(X_corr/Y_corr) * r2d);

		if(X_corr >= 0 && Y_corr >= 0) angle = 180 - angle;
		if(X_corr >= 0 && Y_corr < 0) angle = angle + 180;
		if(X_corr < 0 && Y_corr < 0) angle = 360 - angle;

		print_uart0("headneu %d",
							(signed int)X_corr/*,
							(signed int)Y_corr,
							(signed int)angle*/
						);
		print_uart0(";00#");


		float gx,gy,gz;

		gx = (float)(MM3_runtime.nick / 64.0);
		gy = (float)(MM3_runtime.roll / 64.0);
		gz = (float)(MM3_runtime.ground / 64.0);

		signed int mx,my,mz;

		mx = -MM3_runtime.X_axis;
		my = -MM3_runtime.Y_axis;
		mz = -MM3_runtime.Z_axis;
		gx = gx;
		gy = -gy;
		gz = -gz;

		float hx = (float)(gz*gz * mx) - (float)(gx*gz*mz) - (float)(gx *gy * my) + (float)(gy*gy * mx);    //horizontal X mag vector
	    float hy = (float)(gy * mz) - (float)(gz * my);                                       //horizontal Y mag vector
		angle = (signed int)(float)(atan(hx/hy)*r2d);

	    if (angle > 0)
	    	angle = 360 - angle;
		else
			angle = -angle;


		print_uart0("head2 %d",
					/*(signed int)X_corr,
					(signed int)Y_corr,*/
					(signed int)angle
				);
		print_uart0(";00#");

	MM3_runtime.heading = angle;
	//return (int)(compassHeading);
}

/*
Calculate Roll & Pitch:
Gtot = Isx * Isx
Tmp = Isy * Isy
Gtot = Gtot + Tmp
Tmp = Isz * Isz
Gtot = Gtot + Tmp
Gtot = Sqr(Gtot)
Pitch = Isx / Gtot
Pitch = -Asin(pitch)
Tmp = Cos(pitch)
Tmp = Tmp * Gtot
Tmp = Isy / Tmp
Roll = Asin(Tmp)

Calculate modified magnetic x & y and heading:
Tmp = Magx * Cos(Pitch)
Tmp1= Magy * Sin(Roll)
Tmp1 = Tmp1 * Sin(Pitch)
Tmp = Tmp + Tmp1
Tmp1 = Magz * Cos(Roll)
Tmp1 = Tmp1 * Sin(pitch)
Magxm = Tmp - Tmp1
Tmp = Magy * Cos(Roll)
Tmp1 = Magz * Sin(Roll)
Magym = Tmp + Tmp1
Maghead = Magym / Magxm
Maghead = Atn(maghead)
Maghead = Rad2deg(maghead)
Maghead = Abs(maghead)
If Magxm >= 0 And Magym >= 0 Then Maghead = 180 - Maghead
If Magxm >= 0 And Magym < 0 Then Maghead = Maghead + 180
If Magxm < 0 And Magym < 0 Then Maghead = 360 - Maghead
 */



/*
	 float gx = xraw/64.0; // scale output although, it would be much nicer to
      float gy = yraw/64.0; //   have a calibrated scale and offset for each axis
      float gz = zraw/64.0; //   but for now this will have to do

      spi.format(8,0);      // the MicroMag3 expects cpol=0, cpha=0

      SSnMag = 0;           // Select MicroMag 3

      RstMag = 1; RstMag = 0;         // Mag reset pulse. this creates ~1.1uS pulse
      spi.write(MagCommand + x);      // send request for X axis mag value
      while(!DrdyMag);                // wait for it...
      mx =spi.write(0)*0x100; mx = mx + spi.write(0); // I could not get the spi.format(16,0) to work
                                                      // so I am constructing the word from 2 bytes
      if ( mx > 0x7fff)               // convert to signed value
          mx = mx - 0x10000;
      RstMag = 1; RstMag = 0; // get Y axis mag value
      spi.write(MagCommand + y);
      while(!DrdyMag);
      my =spi.write(0)*0x100; my = my + spi.write(0);
      if ( my > 0x7fff)
          my = my - 0x10000;
      RstMag = 1; RstMag = 0; //get Z axis mag value
      spi.write(MagCommand + z);
      while(!DrdyMag);
      mz =spi.write(0)*0x100; mz = mz + spi.write(0);
      if ( mz > 0x7fff)
          mz = mz - 0x10000;
      SSnMag = 0; // Deselect Mag

      // Axis adjustments. This section adjusts the Axes into a NED (North East Down) body frame
      mx = -mx;
      my = -my;
      mz = -mz;
      gx = -gx;
      gz = -gz;
// I'm using the vector method of tilt compensation rather than a cosine matrix. The vector method does not involve
// a lot of trig. Basically what is going on is that horizontal plane is constructed using Cross products of g and m.
// Then by using Dot products, the body coordinate frame is projected onto the newly contructed horizontal XY vectors.
// for more information search: "A New Solution Algorithm of Magnetic Amizuth", or "A combined electronic compass/clinometer"
      float hx = (gz*gz * mx) - (gx*gz*mz) - (gx *gy * my) + (gy*gy * mx);    //horizontal X mag vector
      float hy = (gy * mz) - (gz * my);                                       //horizontal Y mag vector
      float heading = atan2(hy,hx)*Rad2Deg;
//  {+45}-------{ 0 }-------{-45}  this is the output of atan2(hy,hx)
//      | +-----[ +x]-----+ |      it must be adjusted to convert it
//      | |               | |      to navigational directions
//      | |               | |
//  {+90} [+y]  [0,0]  [-y] {-90} (Note: you get positive y when body frame
//      | |               | |      faces West in an NED system because
//      | |               | |      NED refers to sensor direction when the
//      | +-----[ -x]-----+ |      body frame is at rest in its reference orientation)
// {+135}-----{+/-180}------{-135}
      if (heading > 0)
          heading = 360 - heading;
      else
          heading = -heading;
//  {335}------{360/0}------{45}
//      | +-----[ N ]-----+ |
//      | |               | |      navigational directions
//      | |               | |
//  {270} [W]   [0,0]   [E] {90}
//      | |               | |
//      | |               | |
//      | +------[S]------+ |
//  {225}-------{180}-------{135}

      float pitch = atan(-gx/sqrt(gy*gy+gz*gz))*Rad2Deg; //invert gx because +pitch is up. range is +/-90 degrees
      float roll = atan(gy/sqrt(gx*gx+gz*gz))*Rad2Deg;   // right side down is +roll
      if (gz > 0)                // unfold atan's limited 2 quadrants to extend roll to 4 quadrants (+/-180)
          if ( roll > 0)
              roll = 180 - roll;
          else
              roll = -180 - roll;
      pc.printf("\ngx: %f, gy: %f, gz: %f | mx: %d, my: %d, mz: %d\n",gx,gy,gz,mx,my,mz);
      pc.printf("hx: %f, hy: %f\n",hx,hy);
      pc.printf("heading: %f, pitch: %f, roll: %f\n",heading,pitch,roll);

 * */

