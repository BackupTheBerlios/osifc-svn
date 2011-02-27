#include <nav/nemaparser.h>
#include <arch/led.h>
#include <string.h>
/*
	file    nemaparser.c
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
/**
@mfunc	parse a GPGGA sentence. another complex bugger
@parm	zero terminated data string
@rdesc	true if the data has changed something
//NEMA
//$GPGGA,hhmmss.sec,Latitude,N,Longitude,E,FS,NoSV,HDOP,msl,m,Altref,m,DiffAge,DiffStation*cs<CR><LF>
//$GPGGA,092725.00,4717.11399,N,00833.91590,E,1,8,1.01,499.6,M,48.0,M,,0*5B
//$GPVTG,cogt,T,cogm,M,sog,N,kph,K,mode*cs<CR><LF>
//$GPVTG,77.52,T,,M,0.004,N,0.008,K,A*06

$GPGGA,142917,,,,,0,00,,,M,,M,,*6E
$GPGGA,142957,5127.736,N,00235.708,W,1,08,2.0,-128.5,M,50.0,M,,*78
$GPGGA,143348,5127.736,N,00235.708,W,1,09,2.0,-128.5,M,50.0,M,,*7C

*/
; /*
   ** GGA - Global Positioning System Fix Data
   ** Time, Position and fix related data form GPS receiver.
   **
   **                                                      11
   **        1         2       3 4        5 6 7  8   9  10 |  12 13  14   15
   **        |         |       | |        | | |  |   |   | |   | |   |    |
   ** $--GGA,hhmmss.sec,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
   **
   ** Field Number:
   **  1) Universal Time Coordinated (UTC)
   **  2) Latitude
   **  3) N or S (North or South)
   **  4) Longitude
   **  5) E or W (East or West)
   **  6) GPS Quality Indicator,
   **     0 - fix not available,
   **     1 - GPS fix,
   **     2 - Differential GPS fix
   **  7) Number of satellites in view, 00 - 12
   **  8) Horizontal Dilution of precision
   **  9) Antenna Altitude above/below mean-sea-level (geoid)
   ** 10) Units of antenna altitude, meters
   ** 11) Geoidal separation, the difference between the WGS-84 earth
   **     ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level
   **     below ellipsoid
   ** 12) Units of geoidal separation, meters
   ** 13) Age of differential GPS data, time in seconds since last SC104
   **     type 1 or 9 update, null field when DGPS is not used
   ** 14) Differential reference station ID, 0000-1023
   ** 15) Checksum
   */

//=======================================================================


#include "arch/clock-arch.h"


volatile char NEMA_CK_A, NEMA_CK_B, msgLength;

int NEMAmessageNum = 0;
volatile long NEMATemp = 0;
volatile char *NEMATempref;


GPGGAtime nemaGPGGAtimeRuntime;

char *nemaGPGGAtimeRef = (char *)&nemaGPGGAtimeRuntime;




const unsigned char token = 0x2c;
const unsigned char dot = 0x2e;
const unsigned char NEMAmaxmsgLen = 10;

unsigned char 		dotCount 		= 0;
signed int 			multiplier 		= 1;
unsigned char 		NEMAState 		= 0;
float 				NEMATempfloat 	= 0;
unsigned int 		NEMATempInt 	= 0;
unsigned char 		NEMAmsgLen 		= 0;


int F_RTC_Year = 2011;


GPGGA GPGGAMessage;


void updateNEMA (void) {
	nemaGPGGA = GPGGAMessage;
}

void updateNEMATime (void) {
	nemaGPGGAtime = nemaGPGGAtimeRuntime;
}

void updateRTCtoNEMA(void){
	RTCTime s_RTCTime;
	s_RTCTime.RTC_Hour = (unsigned long)nemaGPGGAtime.hh;
	s_RTCTime.RTC_Min = (unsigned long)nemaGPGGAtime.mm;
	s_RTCTime.RTC_Sec = (unsigned long)nemaGPGGAtime.sec;
	s_RTCTime.RTC_Wday = 1;
	s_RTCTime.RTC_Mday = 1;
	s_RTCTime.RTC_Yday = 1;
	s_RTCTime.RTC_Mon = 1;
	s_RTCTime.RTC_Year = (unsigned long)F_RTC_Year;
	RTCSetTime(s_RTCTime);
}

void clearNemaTemp(void);
void clearNemaTemp(void)
{
	NEMATemp = 0;
	NEMATempref = (volatile char*)&NEMATemp;
	dotCount = 0;
	multiplier = 1;
	NEMATempfloat = 0;
	NEMATempInt = 0;
	NEMAmsgLen = 0;

}
char checkDot(unsigned char c);
char checkDot(unsigned char c)
{
	char myReturn = 0;
	if (c == 0x24){
		dotCount = 1;
		myReturn = 1;
	}
	if(dotCount != 0)
		dotCount *= 10;
	return myReturn;
}
unsigned char checkMulti(unsigned char c);
unsigned char checkMulti(unsigned char c)
{
	char myReturn = 0;
	if (c == 0x2D){
		multiplier = -1;
		myReturn = 1;
	}
	return myReturn;
}

char timeCount = 0;
char UTCpos = 0;
char timeTemp = 0;

void NEMAUTCtoTime(char);
void NEMAUTCtoTime(char c)
{
	if (++timeCount == 1) {
		timeTemp = c;
	}else{
		timeCount = 0;
		*nemaGPGGAtimeRef++ = (char)((timeTemp<<4) + ((char)c - 0x30));
	}
}

char NEMAFillfloat(char);
char NEMAFillfloat(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NEMAmsgLen++ <= floatlen) {
			if (checkDot(c) == 0)
				NEMATempfloat = NEMATempfloat * 10 + ((char)c - 0x30);
		} else {
			NEMAmessageNum++;
			myReturn = 1;
		}
	} else {
			NEMAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}
char NEMAFillchar(char);
char NEMAFillchar(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NEMAmsgLen++ <= charlen) {
			*NEMATempref++ = c;
		} else {
			NEMAmessageNum++;
			myReturn = 1;
		}
	} else {
			NEMAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}
char NEMAFillint(char);
char NEMAFillint(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NEMAmsgLen++ <= intlen) {
			NEMATempInt = NEMATempInt * 10 + ((char)c - 0x30);
		} else {
			NEMAmessageNum++;
			myReturn = 1;
		}
	} else {
			NEMAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}
char NEMAFilllong(char);
char NEMAFilllong(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NEMAmsgLen++ <= longlen) {
			NEMATemp = NEMATemp * 10 + ((char)c - 0x30);
		} else {
			NEMAmessageNum++;
			myReturn = 1;
		}
	} else {
			NEMAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}


float getNEMAfloat(float);
float getNEMAfloat(float value)
{
	if(dotCount == 0)
		dotCount = 1;
	return (value/dotCount)*multiplier;
}



void parseNEMA (unsigned char c){
	if (NEMAdone == 0) {
		switch (NEMAState)
		{
		case NEMA_EMPTY:
			if (c == 0x24)
				NEMAState = NEMA_SYNC1;
			else
				NEMAState = NEMA_EMPTY;
		break;
		case NEMA_SYNC1:
			if (c == 0x47)
				NEMAState = NEMA_SYNC2;
			else
				NEMAState = NEMA_EMPTY;
		break;
		case NEMA_SYNC2:
			if (c == 0x50)
				NEMAState = NEMA_SYNC3;
			else
				NEMAState = NEMA_EMPTY;
		break;
		case NEMA_SYNC3:
			if (c == 0x47)
				NEMAState = NEMA_SYNC4;
			else
				NEMAState = NEMA_EMPTY;
		break;
		case NEMA_SYNC4:
			if (c == 0x47)
				NEMAState = NEMA_SYNC5;
			else
				NEMAState = NEMA_EMPTY;
		break;
		case NEMA_SYNC5:
			if (c == 0x41)
				NEMAState = NEMA_SYNC6;
			else
				NEMAState = NEMA_EMPTY;
		break;

		case NEMA_SYNC6:
			if (c == token) {
				NEMAState = NEMA_FILLING;
				NEMAmessageNum = 0;
				clearNemaTemp();
				nemaGPGGAtimeRef = (char *)&nemaGPGGAtime;
				NEMA_CK_A = 0x01;
				NEMA_CK_B = 0x01;
			} else {
				NEMAState = NEMA_EMPTY;
			}
		break;

		case NEMA_FILLING:
			NEMA_CK_A += c;
			NEMA_CK_B += NEMA_CK_A;
			if (NEMAmsgLen > NEMAmaxmsgLen) {
				NEMAmessageNum = 0;
				clearNemaTemp();
				NEMAState = NEMA_EMPTY;
			}
			// check if we get a "-" indicating a negative float to come like in -123.4
			if (checkMulti(c) == 0) {
				// main filling state machine
				switch (NEMAmessageNum)
				{
					case NEMA_FILL_UTC:
						if (checkDot(c) == 0)
							NEMAUTCtoTime(c);
						if (NEMAFillfloat(c) == 1) {
							GPGGAMessage.UTC = getNEMAfloat(NEMATempfloat);
							updateNEMATime();
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_LAT:
						if (NEMAFillfloat(c) == 1) {
							GPGGAMessage.lat = getNEMAfloat(NEMATempfloat);
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_NORTH:
						if (NEMAFillchar(c) == 1) {
							GPGGAMessage.north = (char)NEMATemp;
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_LON:
						if (NEMAFillfloat(c) == 1) {
							GPGGAMessage.lon = getNEMAfloat(NEMATempfloat);
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_WEST:
						if (NEMAFillchar(c) == 1) {
							GPGGAMessage.west = (char)NEMATemp;
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_GPSQ:
						if (NEMAFillchar(c) == 1) {
							GPGGAMessage.GPSQuality = (char)NEMATemp;
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_NUMSAT:
						if (NEMAFillint(c) == 1) {
							GPGGAMessage.numSat = (int)NEMATempInt;
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_HDP:
						if (NEMAFillfloat(c) == 1) {
							GPGGAMessage.HDOP = getNEMAfloat(NEMATempfloat);
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_ALT:
						if (NEMAFillfloat(c) == 1) {
							GPGGAMessage.alt = getNEMAfloat(NEMATempfloat);
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_ALTM:
						if (NEMAFillchar(c) == 1) {
							GPGGAMessage.altM = (char)NEMATemp;
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_GEOS:
						if (NEMAFillfloat(c) == 1) {
							GPGGAMessage.geoSep = getNEMAfloat(NEMATempfloat);
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_GEOSM:
						if (NEMAFillchar(c) == 1) {
							GPGGAMessage.geoSepM = (char)NEMATemp;
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_DGAGE:
						if (NEMAFillfloat(c) == 1) {
							GPGGAMessage.DGPSage = getNEMAfloat(NEMATempfloat);
							clearNemaTemp();
						}
					break;
					case NEMA_FILL_STID:
						if (NEMAFilllong(c) == 1) {
							GPGGAMessage.stationID = (long)NEMATemp;
							NEMAmessageNum = 0;		//end of message reached
							clearNemaTemp();
							NEMAState = NEMA_ENDC;
							led_switch(3);
						}
					break;
					default:
						NEMAmessageNum = 0;
					}// end fill switch
			}	// end if "-"
		break;

		case NEMA_ENDC:
			if (c == 0x2A)
				NEMAState = NEMA_STOPL;
			else
				NEMAState = NEMA_EMPTY;
		break;
		case NEMA_STOPL:
			NEMA_CK_A = c;
			NEMAState = NEMA_STOPH;
		break;
		case NEMA_STOPH:
			NEMA_CK_A = (NEMA_CK_A<<4) + ((c<<4) / 10);
			if (NEMA_CK_A == NEMA_CK_B)	{		//CS OK last Byte-> packet received successfully
				NEMAState = NEMA_EMPTY;		//ready for next packet
				NEMAdone = 1;				// OK NEMA receive Flag
				led_switch(4);
			}
		break;
		default:
			NEMAState = NEMA_EMPTY;		//ready for next packet
		}
	}
}








