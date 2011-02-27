/*
	file    pidFC.c
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

#ifndef PIDFC_H_
#define PIDFC_H_

signed int Y_Difference_old;
signed int Y_DifferenceTotal;
signed int Y_Difference_Sum;

signed int X_Difference_old;
signed int X_DifferenceTotal;
signed int X_Difference_Sum;

signed int DifferencePitchFact;
signed int Pitch_Difference;
signed int Pitch_Difference_Sum;
signed int Pitch_Difference_old;

signed int DifferencePitchGyr;

signed int Y_Difference_AccY;
signed int Y_Difference_GyrN;

signed int X_Difference_AccX;
signed int X_Difference_GyrR;
signed int Difference, X_DifferenceFact, Y_DifferenceFact;
signed int Y_Response;
signed int Y_Gyr_ACC_Difference;

signed int X_Response;
signed int X_Gyr_ACC_Difference;


void pidDriver (void);

#endif /*PIDFC_H_*/
