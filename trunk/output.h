/*
	file    output.h
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


#ifndef OUTPUT_H_
#define OUTPUT_H_


unsigned int CycleCount;
unsigned int lastCycleCount;
unsigned int printCount;
unsigned int refreshCount;
unsigned int engineStatus;




void checkIapResult(int resultValue);
void checkI2CResult(int resultValue,int num);

void printDot(void);

void printDebug(void);
void printDebug1(void);
void printDebug2(void);
void printDebug3(void);
void printDebug4(void);
void printDebug5(void);
void printDebug6(void);
void printDebug7(void);

void printSettings(void);

void printTelemetrie(void);

void printLoadSetting(int setNum);

#endif /*OUTPUT_H_*/
