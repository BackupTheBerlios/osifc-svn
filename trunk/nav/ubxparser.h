/*
	file    ubxparser.h
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

#ifndef UBXPARSER_H_
#define UBXPARSER_H_

#define	GPS_SYNC_CHAR1				0xb5
#define	GPS_SYNC_CHAR2				0x62
#define	CLASS_NAV					0x01
#define	MSGID_NAVSOL				0x06
#define	UBX_EMPTY					0
#define	UBX_SYNC1					1
#define	UBX_SYNC2					2
#define	UBX_CLASS					3
#define	UBX_LEN1					4
#define	UBX_LEN2					5
#define	UBX_FILLING					6
#define	UBX_CKA						7
#define	UBX_CKB						8


typedef struct {
	long			ITOW;		//time of week
	long			Frac; 		//Nanoseconds remainder of rounded ms above, range -500000 .. 500000
	short			week;  		//GPS week (GPS time)
	char			GPSfix;		//GPSfix Type, range 0..4
								//0x00 = No Fix
								//0x01 = Dead Reckoning only
								//0x02 = 2D-Fix
								//0x03 = 3D-Fix
								//0x04 = GPS + dead reckoning combined
								//0x05 = Time only fix
								//0x06..0xff: reserved
	char			Flags;		//0x01=GPSfixOK (i.Difference. within DOP & ACC Masks)
								//0x02=DiffSoln (is DGPS used)
								//0x04=WKNSET (is Week Number valid)
								//0x08=TOWSET (is Time of Week valid)
								//0x?0=reserved
	long			ECEF_X;		//ECEF X coordinate
	long			ECEF_Y;		//ECEF Y coordinate
	long			ECEF_Z;		//ECEF Z coordinate
	long			Pacc;		//3D Position Accuracy Estimate
	long			ECEFVX;		//ECEF X velocity
	long			ECEFVY;		//ECEF Y velocity
	long			ECEFVZ;		//ECEF Z velocity
	long			SAcc;		//Speed Accuracy Estimate
	short			PDOP;		//Position DOP
	char			res1;		//reserved
	char			numSV;		//Number of SVs used in Nav Solution
	long			res2;		//reserved
	char			packetStatus;
} NAV_SOL_t;

NAV_SOL_t 	navSol;



volatile unsigned char	ubxState;

void parseUBX (unsigned char c);

#endif /* UBXPARSER_H_ */
