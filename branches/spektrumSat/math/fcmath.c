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

#include "fcmath.h"
#include "../arch/printf_p.h"
#include "stdlib.h"
#include "math.h"
const unsigned int lut_atan[270]  =
{0,1,2,3,5,6,7,8,9,10,11,12,13,15,16,17,18,
19,20,21,22,23,24,25,26,27,27,28,29,30,31,32,33,33,34,35,36,37,37,38,39,39,40,41,
41,42,43,43,44,44,45,46,46,47,47,48,48,49,49,50,50,51,51,52,52,52,53,53,54,54,54,
55,55,56,56,56,57,57,57,58,58,58,59,59,59,60,60,60,60,61,61,61,61,62,62,62,62,63,
63,63,63,64,64,64,64,65,65,65,65,65,66,66,66,66,66,67,67,67,67,67,67,68,68,68,68,
68,68,69,69,69,69,69,69,69,70,70,70,70,70,70,70,70,71,71,71,71,71,71,71,71,72,72,
72,72,72,72,72,72,72,73,73,73,73,73,73,73,73,73,73,74,74,74,74,74,74,74,74,74,74,
74,74,75,75,75,75,75,75,75,75,75,75,75,75,75,76,76,76,76,76,76,76,76,76,76,76,76,
76,76,76,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,78,78,78,78,78,78,78,
78,78,78,78,78,78,78,78,78,78,78,78,78,79,79,79,79,79,79,79,79,79,79,79,79,79,79,
79,79,79,79,79,79,79,79,79,79};

const float lut_sinus_f [91]  =
{0.000,0.017,0.035,0.052,0.070,0.087,0.105,
0.122,0.139,0.156,0.174,0.191,0.208,0.225,0.242,0.259,0.276,0.292,0.309,0.326,
0.342,0.358,0.375,0.391,0.407,0.423,0.438,0.454,0.469,0.485,0.500,0.515,0.530,
0.545,0.559,0.574,0.588,0.602,0.616,0.629,0.643,0.656,0.669,0.682,0.695,0.707,
0.719,0.731,0.743,0.755,0.766,0.777,0.788,0.799,0.809,0.819,0.829,0.839,0.848,
0.857,0.866,0.875,0.883,0.891,0.899,0.906,0.914,0.921,0.927,0.934,0.940,0.946,
0.951,0.956,0.961,0.966,0.970,0.974,0.978,0.982,0.985,0.988,0.990,0.993,0.995,
0.996,0.998,0.999,0.999,1.000,1.000};

const unsigned int pgm_asin[201]  = {0,0,1,1,1,1,2,2,2,3,3,3,3,4,4,4,5,5,5,5,6,
6,6,7,7,7,7,8,8,8,9,9,9,9,10,10,10,11,11,11,12,12,12,12,13,13,13,14,14,14,14,15,
15,15,16,16,16,17,17,17,17,18,18,18,19,19,19,20,20,20,20,21,21,21,22,22,22,23,23,
23,24,24,24,25,25,25,25,26,26,26,27,27,27,28,28,28,29,29,29,30,30,30,31,31,31,32,
32,32,33,33,33,34,34,34,35,35,35,36,36,37,37,37,38,38,38,39,39,39,40,40,41,41,41,
42,42,42,43,43,44,44,44,45,45,46,46,46,47,47,48,48,49,49,49,50,50,51,51,52,52,53,
53,54,54,55,55,56,56,57,57,58,58,59,59,60,60,61,62,62,63,64,64,65,66,66,67,68,68,
69,70,71,72,73,74,75,76,77,79,80,82,84,90};

// Arkustangens2 im Gradmaß
signed int atan2_i(signed int x,  signed int y)
{
	int i,angle;
	signed int m;


	/*print_uart0("atan2 %d;%d",

			(signed int)x,
			(signed int)y
		);
		print_uart0(";00#");

*/

	if (!x && !y) return 0;		//atan2 = 0 für x und y = 0

	if (y < 0) m=-1;
	else m=1;

	if (x==0) return (90*m);		// atan2 = 90° für x = 0

	i = abs(((float)y / x) * 50);		// Berechne i für die Lookup table (Schrittweite atan(x) ist 0.02 -> *50)



	if (i<270) angle = lut_atan[i];	// Lookup für 1° bis 79°
	else if (i>5750) angle = 90;						// Grenzwert ist 90°
	else if (i>=1910) angle = 89;						// 89° bis 80° über Wertebereiche
	else if (i>=1150) angle = 88;
	else if (i>=820) angle = 87;
	else if (i>=640) angle = 86;
	else if (i>=520) angle = 85;


	else if (i>=380) angle = 83;
	else if (i>=335) angle = 82;
	else if (i>=299) angle = 81;
	else angle = 80; // (i>=270)


	/*print_uart0("atan2 %d;%d",
			(signed int)i,
			(signed int)angle
		);
		print_uart0(";00#");
*/
	if (x > 0) return (angle*m);	// Quadrant I und IV
	else if ((x < 0) && (y >= 0)) return ((angle*-1) + 180);	// Quadrant II
	else return (angle - 180); // x < 0 && y < 0	Quadrant III
}
/*
			if(XE==0 && YE <0) azimuthgrad = 90;
			if(XE==0 && YE >0) azimuthgrad = 270;
			if(XE < 0) azimuthgrad = 180-(atan(YE/XE)*180/M_PI);
			if(XE > 0 && YE < 0) azimuthgrad = -(atan(YE/XE)*180/M_PI);
			if(XE > 0 && YE > 0) azimuthgrad = 360 - (atan(YE/XE)*180/M_PI);

*/


// Kosinusfunktion im Gradmaß
float cos_f(float winkel)
{

 return (sin_f(90-winkel));
}

// Sinusfunktion im Gradmaß
float sin_f(float winkel)
{
 signed int m,n,iwinkel;
 float sinus;





 //winkel = winkel % 360;

 if (winkel >= 0)
 {
	// print_uart0("here m = 1 ;00#");
	m = 1;

 }  else {
	// print_uart0("here m = -1 ;00#");
	m = -1;
	winkel = abs(winkel);
 }

 // Quadranten auswerten
 if ((winkel > 90 ) && (winkel <= 180))
 	{winkel = 180 - winkel; n = 1;}
 else if ((winkel > 180 ) && (winkel <= 270))
 	{winkel = winkel - 180; n = -1;}
 else if ((winkel > 270) && (winkel <= 360))
 	{winkel = 360 - winkel; n = -1;}
 else
 	n = 1; //0 - 90 Grad

 sinus = (float)sin(winkel);
//sinus = lut_sinus_f[iwinkel];

print_uart0("sinus %d;%d;%d;%d",
		(signed int)winkel,
		(signed int)sinus * 1000,
		(signed int)m,
		(signed int)n
	);
	print_uart0(";00#");




return (sinus*m*n);
}



// Akurssinusfunktion im Gradmaß
signed int asin_i(signed int i)
{
	signed int m,myasin;

	if (i < 0) {m=-1;i=abs(i);}
	else m=1;
	myasin = pgm_asin[i] * m;
	/*print_uart0("asin %d;%d",
			(signed int)asin,
			(signed int)m
		);
		print_uart0(";00#");*/
	return (myasin);
}



