/*
	file    compass.h
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


#ifndef COMPASS_H
#define COMPASS_H



typedef struct __attribute__ ((__aligned__))
{
	char STATE;
	signed int X_axis;
	signed int Y_axis;
	signed int Z_axis;
	signed int X_axis_min;
	signed int X_axis_max;
	int X_axis_range;
	signed int Y_axis_min;
	signed int Y_axis_max;
	int Y_axis_range;
	signed int Z_axis_min;
	signed int Z_axis_max;
	int Z_axis_range;

	signed int heading;
	signed int pitch;
	signed int roll;
} HMCRUNTIME;




volatile HMCRUNTIME HMC_runtime;

int heading;


void setHMC5843Continous(void);
void initialreadHMC5843(void);
void readHMC5843(void);
void setHMC5843Rate(void);
void setHMC5843Gain(void);
void calcHMC5843Offset(void);


void initHMC6343(void);
void readHMC6343(void);


int HMC6343Read;

#endif



