/*
	file    osiGPS.c
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

#include <stdlib.h>
#include <math.h>

#include "osiGPS.h"
#include "gps.h"



/* Here we do the GPS magic
 * GPS should become an AID for beginers or for making fotos.
 * GPS should become a autopilot by following waypoints or come home
 * come home will be at start coordinates.
 * GPS should also enable some funny things like a virtual cell
 *
 *
 *
 */


void initGPS(void)
{
	oldPos = (GPSPOS){0,0,0,GPS_EMPTY};
	actPos = (GPSPOS){0,0,0,GPS_EMPTY};
	homePos = (GPSPOS){0,0,0,GPS_EMPTY};
	targetPos = (GPSPOS){0,0,0,GPS_EMPTY};
	osi_GPS_status = (GPSSTATUS){GPS_MODE_OFF,GPS_EMPTY,0,0};

}





GPSPOSRUNTIME ecef2lla(int x,int y,int z) {

	//function [lat,lon,alt] = ecef2lla(x,y,z)

	//WGS84 ellipsoid constants:
	long a = 6378137;
	float e = 298.257223563;
	//float deg = 0.017453292519943295769236907684886;

	//calculations:
	double b   = sqrt(pow(a,2)*(1-pow(e,2)));
	float ep  = sqrt((pow(a,2)-pow(b,2))/pow(b,2));
	float p   = sqrt(pow(x,2)+pow(y,2));
	float th  = atan2(a*z,b*p);
	float lon = atan2(y,x);
	float lat = atan2((z+pow(ep,2)*b*pow(sin(th),3)),(p-pow(e,2)*a*pow(cos(th),3)));
	float N   = a/sqrt(1-pow(e,2)*pow(sin(lat),2));
	float alt = p/cos(lat)-N;

	//return lon in range [0,2*pi)
	//lon = mod(lon,2*pi);

	//correct for numerical instability in altitude near exact poles:
	//(after this correction, error is about 2 millimeters, which is about
	//the same as the numerical precision of the overall function)

	//int k=abs(x)<1 & abs(y)<1;
	//alt(k) = abs(z(k))-b;

	GPSPOSRUNTIME position;
	position.longitude = lon;
	position.latitude = lat;
	position.altitude = alt;

	return position;


}



