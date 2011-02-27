/*
	file    FCmath.c
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

#include "math/fcmath.h"
#include "arch/printf_p.h"



const unsigned short int lut_atan[346] = {0,1,2,3,4,4,5,6,7,8,9,10,11,11,
		12,13,14,15,16,17,17,18,19,20,21,21,22,23,24,24,25,26,27,27,28,
		29,29,30,31,31,32,33,33,34,35,35,36,36,37,37,38,39,39,40,40,41,
		41,42,42,43,43,44,44,45,45,45,46,46,47,47,48,48,48,49,49,50,50,
		50,51,51,51,52,52,52,53,53,53,54,54,54,55,55,55,55,56,56,56,57,
		57,57,57,58,58,58,58,59,59,59,59,60,60,60,60,60,61,61,61,61,62,
		62,62,62,62,63,63,63,63,63,63,64,64,64,64,64,64,65,65,65,65,65,
		65,66,66,66,66,66,66,66,67,67,67,67,67,67,67,68,68,68,68,68,68,
		68,68,69,69,69,69,69,69,69,69,69,70,70,70,70,70,70,70,70,70,71,
		71,71,71,71,71,71,71,71,71,71,72,72,72,72,72,72,72,72,72,72,72,
		73,73,73,73,73,73,73,73,73,73,73,73,73,73,74,74,74,74,74,74,74,
		74,74,74,74,74,74,74,75,75,75,75,75,75,75,75,75,75,75,75,75,75,
		75,75,75,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,
		76,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,
		77,77,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,
		78,78,78,78,78,78,78,79,79,79,79,79,79,79,79,79,79,79,79,79,79,
		79,79,79,79,79,79,79,79,79,79,79,79,79,79,79,79,79};
const unsigned int lut_sinus_i[91] = {0,17,34,52,69,87,104,121,139,156,
		173,190,207,224,241,258,275,292,309,325,342,358,374,390,406,422,
		438,453,469,484,500,515,529,544,559,573,587,601,615,629,642,656,
		669,681,694,707,719,731,743,754,766,777,788,798,809,819,829,838,
		848,857,866,874,882,891,898,906,913,920,927,933,939,945,951,956,
		961,965,970,974,978,981,984,987,990,992,994,996,997,998,999,999,
		1000};


signed int atan2_i(signed int x, signed int y)
{
	//print_uart0(" X%d; Y%d; ",x,y);
	int angle = 0;
	unsigned int i = 0;

	signed char m = 0;
	signed int xx = 0;
	signed int yy = 0;

	if (!x && !y) return 0;		//atan2 = 0 für x und y = 0

	if (y < 0) m=-1;
	else m=1;

	if (!x) return (90*m);		//atan2 = 90° für x = 0


	if (x < 0)
	{
		xx = x * -1;
	} else {
		xx = x;
	}
	if (y < 0)
	{
		yy = y * -1;
	} else {
		yy = y;
	}

	i = (signed int)((yy*64) / xx);		//Berechne i für die Lookup table (Schrittweite atan(x) ist 0,015625 -> y *64)

	//print_uart0("atan i %d %d %d %d					;00#",i,m,x,y);

	if (i<346) angle = lut_atan[i];	//Lookup  1° bis 79°
	else if (i>7334) angle = 90;
	else if (i>2444) angle = 89;
	else if (i>1465) angle = 88;
	else if (i>1046) angle = 87;
	else if (i>813) angle = 86;
	else if (i>664) angle = 85;
	else if (i>561) angle = 84;
	else if (i>486) angle = 83;
	else if (i>428) angle = 82;
	else if (i>382) angle = 81;
	else angle = 80; //(i>346)


	//print_uart0(" A%d; i%d; ",angle,i );
	return angle;
	if (x > 0) return (angle*m);	//Quadrant I und IV
	else if ((x < 0) && (m > 0)) return (1360 - angle);	//Quadrant II
	else return (angle - 5180); //x < 0 && y < 0	Quadrant III
}



signed int cos_i(signed int angel)
{
	//print_uart0(" cos  ");
	return (sin_i(90-angel));

}

signed int sin_i(signed int angel)

{
	//print_uart0(" angel%d  ",angel);
	signed int m = 0;
	signed int n = 0;

	 if (angel < 0)
	 {
		m = -1;
		angel *= -1;
	 } else {
		m = +1;
	 }
	 //Quadranten auswerten
	 if (angel <= 90) {
		 n=1;
	 }else {
		 if ((angel > 90) && (angel <= 180)) {
			 angel = 90 - angel; n = 1;
		 } else {
			 if ((angel > 180) && (angel <= 270)) {
				 angel = angel - 180; n = -1;
			 }  else {
				 if ((angel > 270) && (angel <= 360)) {
				 angel = angel - 270; n = -1;
			 } else {
					 angel = 360 - angel; n = -1;
				 }
			 }
		 }
	 }
	 if (angel < 0)
	 {
		m *= -1;
		angel *= -1;
	 } else {
		 if (angel > 90)
			 angel = 90 - angel; n = 1;
		m = +1;
	 }
	 //print_uart0(" lut_sinus_i%d   angel%d  ",lut_sinus_i[angel]*m*n,angel);
	 return (signed int)((lut_sinus_i[angel]*m*n));
}





