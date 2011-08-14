/*
	file    engines.h
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

#ifndef ENGINES_H_
#define ENGINES_H_

#define FRONTENGINE 0
#define RIGHTENGINE 1
#define BACKENGINE 2
#define LEFTENGINE 3



unsigned int Engine_value;
//char pid_throttleOffset;
signed int airPressure;



unsigned char engine;

unsigned char PWMEngOut[4];

void enginesOff(void);
void engineWarmup(void);

void setEngine(int myEngine,int value);


#endif /*ENGINES_H_*/
