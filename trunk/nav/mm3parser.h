/*
	file    mm3parser.h
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
#ifndef MM3_H_
#define MM3_H_

//MM3 - Configuration
#define MM3_X_AXIS		0x01
#define MM3_Y_AXIS		0x02
#define MM3_Z_AXIS		0x03

#define MM3_PERIOD_32	0x00
#define MM3_PERIOD_64	0x10
#define MM3_PERIOD_128	0x20
#define MM3_PERIOD_256	0x30
#define MM3_PERIOD_512	0x40
#define MM3_PERIOD_1024 0x50
#define MM3_PERIOD_2048 0x60
#define MM3_PERIOD_4096 0x70

//toDo make this a setting
#define MM3_PS	MM3_PERIOD_512 //MM3 Period select to select how many ticks to count during measurement
								//2048 is the largest value safe not to count till overflow

//states for MM3
#define MM3_RESET			0
#define MM3_START_TRANSFER	1
#define MM3_DRDY			2
#define MM3_X				3
#define MM3_Y				4
#define MM3_Z				5
#define MM3_TILT			6
#define MM3_IDLE			7
#define MM3_READNOW			8
#define MM3_WAIT_DRDY		9
#define MM3_WAIT_HB			10
#define MM3_WAIT_LB			11

#define MM3_OFF = IO0_OFF
#define MM3_resetDelayCycles 10  				//this defines the MM3 reset pulse length



typedef struct __attribute__ ((__aligned__))
{
	char STATE;
	char AXIS;
	signed int heading;
	signed int X_axis;
	signed int Y_axis;
	signed int Z_axis;
	signed int roll;
	signed int nick;
	signed int ground;
} MM3RUNTIME;




volatile char calib;


volatile MM3RUNTIME MM3_runtime;

void setMM3State(char state);
void mm3calcHeading(void);
void parseMM3(void);
int heading_MM3(void);
void calib_MM3(void);










#endif /* MM3_H_ */
