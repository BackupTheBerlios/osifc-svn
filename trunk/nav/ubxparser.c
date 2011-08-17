/*
	file    ubxparser.c
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

#include "ubxparser.h"
#include "../arch/led.h"
#include "gps.h"

volatile unsigned char	ignorePacket;		//true when previous packet was not processed
volatile unsigned short	UBXmsgLen;

//UBX UART2
volatile char			*ubxP, *ubxEp, *ubxSp;	//pointers to packet currently transfered
volatile unsigned char		CK_A, CK_B;			//UBX checkSum bytes
volatile unsigned char		msgID;

void parseUBX (unsigned char c){
	switch (ubxState)
	{
	case UBX_EMPTY:
		if (c == GPS_SYNC_CHAR1)
			ubxState = UBX_SYNC1;
		break;
	case UBX_SYNC1:
		if (c == GPS_SYNC_CHAR2)
			ubxState = UBX_SYNC2;

		else if (c != GPS_SYNC_CHAR1)
			ubxState = UBX_EMPTY;
		break;
	case UBX_SYNC2:
		if (c == CLASS_NAV)
			ubxState = UBX_CLASS;
		else
			ubxState = UBX_EMPTY;
		break;
	case UBX_CLASS:					//msg ID seen: init packed receive
		msgID = c;
		CK_A = CLASS_NAV + c;
		CK_B = CLASS_NAV + CK_A;
		ubxState = UBX_LEN1;

		switch (msgID)
		{
			case MSGID_NAVSOL:
				ubxP = (char*)&navSol;
				ubxEp = (char*)(&navSol + sizeof(NAV_SOL_t));
				ubxSp = (char*)&navSol.packetStatus;
				ignorePacket = navSol.packetStatus;
			break;
			default:
				ignorePacket = 1;
				ubxSp = (char*)0;
		}
	break;
	case UBX_LEN1:					//first len byte
		UBXmsgLen = c;
		CK_A += c;
		CK_B += CK_A;
		ubxState = UBX_LEN2;
		break;
	case UBX_LEN2:					//second len byte
		UBXmsgLen = UBXmsgLen + (c * 256);
		CK_A += c;
		CK_B += CK_A;
		ubxState = UBX_FILLING;		//next data will be stored in packet struct
		break;
	case UBX_FILLING:
		CK_A += c;
		CK_B += CK_A;
		if ( !ignorePacket && ubxP < ubxEp)
				*ubxP++ = c;
		if (--UBXmsgLen == 0)
			ubxState = UBX_CKA;
	break;
	case UBX_CKA:
		if (c == CK_A)
			ubxState = UBX_CKB;
		else
			ubxState = UBX_EMPTY;
	break;
	case UBX_CKB:
		if (c == CK_B && ubxSp)		//No error -> packet received successfully
			*ubxSp = 1;				//set packetStatus in struct
		ubxState = UBX_EMPTY;		//ready for next packet
		led_switch(4);
	break;
	default:
		ubxState = UBX_EMPTY;		//ready for next packet
	}

}
