/*
	file    FCmath.h
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


#ifndef FCMATH_H
#define FCMATH_H

#define r2d 				57.29577951308232087679815481411			//= 180 / PI
#define d2r 				0.017453292519943295769236907684			//= PI / 180
#define PI        			3.141592653589793238462643383279
#define HALFPI    			1.570796326794896619231321691639
#define QUARTERPI			0.785398163397448309615660845819
#define TWOPI     			6.283185307179586476925286766559
#define SECONDS_IN_WEEK 	604800
#define LIGHTSPEED  		299792458


signed int cos_i(signed int winkel);
signed int sin_i(signed int winkel);
signed int atan2_i(signed int x, signed int y);

#endif /*FCMATH_H*/
