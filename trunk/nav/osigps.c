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

#include "nav/osiGPS.h"
#include "nav/gps.h"

#include "nav/NMEAparser.h"
#include "nav/ubxparser.h"
#include "interface/command.h"

char GPStype = 0;
const char NMEA = 0;
const char UBX = 1;

/* Here we do the GPS magic
 * GPS should become an AID for beginners or for making fotos.
 * GPS should become a autopilot by following way points or come home
 * come home will be at start flight coordinates.
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
	NMEAdone = 0;
}

// called from uart2 irq getting the received char c
void parseGPS(unsigned char c)
{




	if(GPStype == NMEA) {
		if (NMEAdone == 0)
			parseNMEA(c);

	} else {
		if(GPStype == UBX) {
			parseUBX(c);
		}
	}

}
