/*
	file    NMEAparser.h
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

#ifndef NMEAPARSER_H_
#define NMEAPARSER_H_

#define	NMEA_EMPTY					0
#define	NMEA_SYNC1					1
#define	NMEA_SYNC2					2
#define	NMEA_SYNC3					3
#define	NMEA_SYNC4					4
#define	NMEA_SYNC5					5
#define	NMEA_SYNC6					6

#define	NMEA_CLASS					7
#define	NMEA_LEN1					8
#define	NMEA_LEN2					9
#define	NMEA_FILLING				10
#define	NMEA_ENDC					11
#define	NMEA_STOPL					12
#define	NMEA_STOPH					13

#define	NMEA_FILL_UTC					0
#define	NMEA_FILL_LAT					1
#define	NMEA_FILL_LON					2
#define	NMEA_FILL_NORTH					3
#define	NMEA_FILL_WEST					4
#define	NMEA_FILL_GPSQ					5
#define	NMEA_FILL_NUMSAT				6
#define	NMEA_FILL_HDP					7
#define	NMEA_FILL_ALT					8
#define	NMEA_FILL_ALTM					9
#define	NMEA_FILL_GEOS					10
#define	NMEA_FILL_GEOSM					11
#define	NMEA_FILL_DGAGE					12
#define	NMEA_FILL_STID					13


#define	STRING_VALUE_LENGTH				10

#define floatlen   8
#define intlen   2
#define longlen   floatlen
#define charlen   1



typedef struct __attribute__ ((__aligned__))
{
	float UTC;
	float lat;
	float lon;
	char north;
	char west;
	char GPSQuality;
	int numSat;
	float HDOP;
	float alt;
	char altM;
	float geoSep;
	char geoSepM;
	float DGPSage;
	long stationID;
}GPGGA;

typedef struct __attribute__ ((__aligned__))
{
	char hh;
	char mm;
	char sec;
	char ms;
}GPGGAtime;

GPGGAtime NMEAGPGGAtime;
GPGGA NMEAGPGGA;

char NMEAdone;


void updateRTCtoNMEA(void);
void updateNMEATime (void);
void parseNMEA (unsigned char);
void updateNMEA (void);


#endif /* NMEAPARSER_H_ */
