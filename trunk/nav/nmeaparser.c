/*
	file    NMEAparser.c
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
//NMEA
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

#include "NMEAparser.h"
#include "MM3parser.h"
#include "../arch/clock-arch.h"
#include "../arch/printf_p.h"
#include "../arch/led.h"
#include "../io/pwmin.h"
#include <string.h>



volatile unsigned char NMEA_CK_A, NMEA_CK_B, msgLength;

int NMEAmessageNum = 0;
volatile long NMEATemp = 0;
volatile char *NMEATempref;


GPGGAtime NMEAGPGGAtimeRuntime;

char *NMEAGPGGAtimeRef = (char *)&NMEAGPGGAtimeRuntime;




const unsigned char token = 0x2c;
const unsigned char dot = 0x2e;
const unsigned char NMEAmaxmsgLen = 10;

unsigned char 		dotCount 		= 0;
signed int 			multiplier 		= 1;
unsigned char 		NMEAState 		= 0;
float 				NMEATempfloat 	= 0;
unsigned int 		NMEATempInt 	= 0;
unsigned char 		NMEAmsgLen 		= 0;


int F_RTC_Year = 2011;


GPGGA GPGGAMessage;


void updateNMEA (void) {
	NMEAGPGGA = GPGGAMessage;
}

void updateNMEATime (void) {
	NMEAGPGGAtime = NMEAGPGGAtimeRuntime;
}

void updateRTCtoNMEA(void){
	RTCTime s_RTCTime;
	s_RTCTime.RTC_Hour = (unsigned long)NMEAGPGGAtime.hh;
	s_RTCTime.RTC_Min = (unsigned long)NMEAGPGGAtime.mm;
	s_RTCTime.RTC_Sec = (unsigned long)NMEAGPGGAtime.sec;
	s_RTCTime.RTC_Wday = 1;
	s_RTCTime.RTC_Mday = 1;
	s_RTCTime.RTC_Yday = 1;
	s_RTCTime.RTC_Mon = 1;
	s_RTCTime.RTC_Year = (unsigned long)F_RTC_Year;
	RTC_Set_Time(s_RTCTime);
}

void clearNMEATemp(void);
void clearNMEATemp(void)
{
	NMEATemp = 0;
	NMEATempref = (volatile char*)&NMEATemp;
	dotCount = 0;
	multiplier = 1;
	NMEATempfloat = 0;
	NMEATempInt = 0;
	NMEAmsgLen = 0;

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

void NMEAUTCtoTime(char);
void NMEAUTCtoTime(char c)
{
	if (++timeCount == 1) {
		timeTemp = c;
	}else{
		timeCount = 0;
		*NMEAGPGGAtimeRef++ = (char)((timeTemp<<4) + ((char)c - 0x30));
	}
}

char NMEAFillfloat(char);
char NMEAFillfloat(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NMEAmsgLen++ <= floatlen) {
			if (checkDot(c) == 0)
				NMEATempfloat = NMEATempfloat * 10 + ((char)c - 0x30);
		} else {
			NMEAmessageNum++;
			myReturn = 1;
		}
	} else {

			NMEAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}
char NMEAFillchar(char);
char NMEAFillchar(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NMEAmsgLen++ <= charlen) {
			*NMEATempref++ = c;
		} else {
			NMEAmessageNum++;
			myReturn = 1;
		}
	} else {

			NMEAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}
char NMEAFillint(char);
char NMEAFillint(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NMEAmsgLen++ <= intlen) {
			NMEATempInt = NMEATempInt * 10 + ((char)c - 0x30);
		} else {
			NMEAmessageNum++;
			myReturn = 1;
		}
	} else {

			NMEAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}
char NMEAFilllong(char);
char NMEAFilllong(char c)
{
	char myReturn = 0;
	if (c != token) {
		if (NMEAmsgLen++ <= longlen) {
			NMEATemp = NMEATemp * 10 + ((char)c - 0x30);
		} else {
			NMEAmessageNum++;
			myReturn = 1;
		}
	} else {

			NMEAmessageNum++;
			myReturn = 1;
	}
	return myReturn;
}


float getNMEAfloat(float);
float getNMEAfloat(float value)
{
	if(dotCount == 0)
		dotCount = 1;
	return (value/dotCount)*multiplier;
}


int tmp;
void parseNMEA (unsigned char c){

	if (tmp++ >1000) {
		LED2_OFF;
		tmp = 0;
	} else {
		LED2_ON;
		tmp = 0;
	}
	print_uart0("%2x",c);
	if (NMEAdone == 0) {
		switch (NMEAState)
		{
		case NMEA_EMPTY:
			if (c == 0x24) {
				NMEAState = NMEA_SYNC1;
			}else{
				NMEAState = NMEA_EMPTY;
			}
		break;
		case NMEA_SYNC1:
			if (c == 0x47)
				NMEAState = NMEA_SYNC2;
			else
				NMEAState = NMEA_EMPTY;
		break;
		case NMEA_SYNC2:
			if (c == 0x50)
				NMEAState = NMEA_SYNC3;
			else
				NMEAState = NMEA_EMPTY;
		break;
		case NMEA_SYNC3:
			if (c == 0x47)
				NMEAState = NMEA_SYNC4;
			else
				NMEAState = NMEA_EMPTY;
		break;
		case NMEA_SYNC4:
			if (c == 0x47)
				NMEAState = NMEA_SYNC5;
			else
				NMEAState = NMEA_EMPTY;
		break;
		case NMEA_SYNC5:
			if (c == 0x41)
				NMEAState = NMEA_SYNC6;
			else
				NMEAState = NMEA_EMPTY;
		break;

		case NMEA_SYNC6:
			if (c == token) {
				NMEAState = NMEA_FILLING;
				NMEAmessageNum = 0;
				clearNMEATemp();
				NMEAGPGGAtimeRef = (char *)&NMEAGPGGAtime;
				NMEA_CK_A = 0;
				NMEA_CK_B = 0;
			} else {
				NMEAState = NMEA_EMPTY;
			}
		break;

		case NMEA_FILLING:
			NMEA_CK_A += c;
			NMEA_CK_B = (unsigned char)(NMEA_CK_B ^ c);
			if (NMEAmsgLen > NMEAmaxmsgLen) {
				NMEAmessageNum = 0;
				clearNMEATemp();
				NMEAState = NMEA_EMPTY;

			}
			// check if we get a "-" indicating a negative float to come like in -123.4
			if (checkMulti(c) == 0) {
				// main filling state machine
				switch (NMEAmessageNum)
				{
					case NMEA_FILL_UTC:
						if (checkDot(c) == 0)
							NMEAUTCtoTime(c);
						if (NMEAFillfloat(c) == 1) {
							GPGGAMessage.UTC = getNMEAfloat(NMEATempfloat);
							updateNMEATime();
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_LAT:
						if (NMEAFillfloat(c) == 1) {
							GPGGAMessage.lat = getNMEAfloat(NMEATempfloat);
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_NORTH:
						if (NMEAFillchar(c) == 1) {
							GPGGAMessage.north = (char)NMEATemp;
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_LON:
						if (NMEAFillfloat(c) == 1) {
							GPGGAMessage.lon = getNMEAfloat(NMEATempfloat);
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_WEST:
						if (NMEAFillchar(c) == 1) {
							GPGGAMessage.west = (char)NMEATemp;
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_GPSQ:
						if (NMEAFillchar(c) == 1) {
							GPGGAMessage.GPSQuality = (char)NMEATemp;
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_NUMSAT:
						if (NMEAFillint(c) == 1) {
							GPGGAMessage.numSat = (int)NMEATempInt;
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_HDP:
						if (NMEAFillfloat(c) == 1) {
							GPGGAMessage.HDOP = getNMEAfloat(NMEATempfloat);
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_ALT:
						if (NMEAFillfloat(c) == 1) {
							GPGGAMessage.alt = getNMEAfloat(NMEATempfloat);
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_ALTM:
						if (NMEAFillchar(c) == 1) {
							GPGGAMessage.altM = (char)NMEATemp;
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_GEOS:
						if (NMEAFillfloat(c) == 1) {
							GPGGAMessage.geoSep = getNMEAfloat(NMEATempfloat);
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_GEOSM:
						if (NMEAFillchar(c) == 1) {
							GPGGAMessage.geoSepM = (char)NMEATemp;
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_DGAGE:
						if (NMEAFillfloat(c) == 1) {
							GPGGAMessage.DGPSage = getNMEAfloat(NMEATempfloat);
							clearNMEATemp();
						}
					break;
					case NMEA_FILL_STID:
						if (c == 0x2A){
							NMEAState = NMEA_STOPL;
						}else{
							if (NMEAFilllong(c) == 1) {
								GPGGAMessage.stationID = (long)NMEATemp;
								NMEAmessageNum = 0;		//end of message reached
								clearNMEATemp();
								NMEAState = NMEA_ENDC;

							}
						}
					break;
					default:
						NMEAmessageNum = 0;
					}// end fill switch
			}	// end if "-"
		break;

		case NMEA_ENDC:

			NMEAState = NMEA_STOPL;
			if (c == 0x2A)
				NMEAState = NMEA_STOPL;
			//else
				//NMEAState = NMEA_EMPTY;
		break;
		case NMEA_STOPL:
			NMEA_CK_A = c - 0x30;
			NMEAState = NMEA_STOPH;
		break;
		char tempchar;
		case NMEA_STOPH:

			tempchar = (int)(c - 0x30);

			//tempchar = 15;

			if (tempchar > 10){
				tempchar -= 7;
			}
			NMEA_CK_B += 0x30;
			NMEA_CK_A = (char)(NMEA_CK_A<<4) | tempchar;
			if (NMEA_CK_A == NMEA_CK_B)	{		//CS OK last Byte-> packet received successfully

				NMEAdone = 0;				// OK NMEA receive Flag
				//led_switch(4);

			}
			NMEAState = NMEA_EMPTY;		//ready for next packet
			//print_uart0(" %2x %2x #",(char)GPGGAMessage.numSat ,GPGGAMessage.GPSQuality );
			//print_uart0(" #");
		break;
		default:
			NMEAState = NMEA_EMPTY;		//ready for next packet
		}
	}
}

