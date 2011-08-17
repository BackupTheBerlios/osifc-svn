/*
	file    osiGPS.h
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


#ifndef OSI_GPS_H_
#define OSI_GPS_H_

typedef struct __attribute__ ((__aligned__))
{
	long longitude;
	long latitude;
	long altitude;
	char status;
}GPSPOS;

typedef struct __attribute__ ((__aligned__))
{
	long longitude;
	long latitude;
	long altitude;
}GPSPOSRUNTIME;

typedef struct __attribute__ ((__aligned__))
{
	char GPS_USER_MODE;
	char GPS_sensor_status;
	char GPS_fiX_status;
	char GPS_numSat_mittel;
}GPSSTATUS;


#define			GPS_EMPTY	0
#define			GPS_READY	1
#define			GPS_BUSY	2

#define			GPS_MODE_OFF	0
#define			GPS_MODE_AUTO	1
#define			GPS_MODE_HOME	2
#define			GPS_MODE_HOLD	3
#define			GPS_MODE_AID	4

#define			GPS_FORCE_Max	20
#define			GPS_INTEGRAL_Max	2000
#define			GPS_STICK_Max	10


char GPS_Mode;
char GPS_enabled;
char GPS_delay;

GPSSTATUS osi_GPS_status;

GPSPOS oldPos;
GPSPOS actPos;
GPSPOS homePos;
GPSPOS targetPos;

void initGPS(void);
void parseGPS(unsigned char c);
void checkGPSUart(void);


#endif /*OSI_GPS_H_*/
