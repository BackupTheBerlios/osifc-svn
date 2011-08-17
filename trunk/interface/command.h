/*
	file    command.h
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

#ifndef COMMAND_H
#define COMMAND_H

#include "../arch/printf_p.h"
#include "../io/output.h"


volatile unsigned char	cmdState;
volatile unsigned char	satState;
volatile unsigned char	SettingsReceived;

#define	CMD_EMPTY					0
#define	CMD_SYNC1					1
#define	CMD_SYNC2					2
#define	CMD_DIV						3
#define	CMD_LEN1					4
#define	CMD_LEN2					5
#define	CMD_FILLING					6
#define	CMD_CKA						7
#define	CMD_CKB						8
#define	CMD_REQ						9
#define	CMD_DIV2					10
#define	CMD_SET						11
#define	SET_FILLING					12
#define	SET_DIV						13
#define	SET_STOP					14
#define	UPD_REQ						15
#define	RTR_UPD_VAL					16
#define	RET_SET						17
#define	SET_MSGLENGTH				18
#define	YGE_SRC						19
#define	YGE_WRITE_TARGET_SRC		20
#define	YGE_WRITE_TARGET_TRG		21
#define	YGE_START_TARGET			22
#define	YGE_READ_ADR				23
#define	YGE_READ_TARGET				24
#define	CMD_FULL					99

#define	CMD_NUM0					0
#define	CMD_NUM1					1


#define CMD_PRINT_DEBUG 			0
#define CMD_PRINT_DEBUG1 			1
#define CMD_PRINT_DEBUG2 			2
#define CMD_PRINT_DEBUG3 			3
#define CMD_PRINT_DEBUG4 			4

#define CMD_READ_CALIB_COMPASS 		6
#define CMD_WRITE_CALIB_COMPASS 	7
#define CMD_START_CALIB_COMPASS 	8
#define CMD_STOP_CALIB_COMPASS 		9


#define	CMD_SYNC_CHAR1				0x46
#define	CMD_SYNC_CHAR2				0x43
#define	CMD_SYNC_CHAR3				0x30
#define	CMD_SYNC_DIV				0x26

#define SAT_SYNC_CHAR1				0x03
#define SAT_SYNC_CHAR2				0x01

#define	CMD_DEC_NUM0				0x30
#define	CMD_DEC_NUM1				0x31
#define	CMD_DEC_NUM2				0x32
#define	CMD_DEC_NUM3				0x33
#define	CMD_DEC_NUM4				0x34
#define	CMD_DEC_NUM5				0x35
#define	CMD_DEC_NUM6				0x36
#define	CMD_DEC_NUM7				0x37
#define	CMD_DEC_NUM8				0x38
#define	CMD_DEC_NUM9				0x39

#define	CMD_DEB_REQ					0x44
#define	CMD_SET_REQ					0x53
#define	CMD_UPD_REQ					0x55
#define	CMD_RET_SET					0x52
#define	CMD_SET_YGE					0x59
#define	CMD_START_YGE				0x79
#define	CMD_READ_YGE				0x78

#define	CMD_STOP_CHAR				0x23		//in memory of Karl


#define SAT_START_1					0x03
#define SAT_START_2					0x01

#define SAT_SYNC_1					1
#define SAT_SYNC_2					2
#define SAT_AILE_1					3
#define SAT_AILE_2					4
#define SAT_FLAPS_1					5
#define SAT_FLAPS_2					6
#define SAT_GEAR_1					7
#define SAT_GEAR_2					8
#define SAT_ELEV_1					9
#define SAT_ELEV_2					10
#define SAT_AUX2_1					11
#define SAT_AUX2_2					12
#define SAT_THRO_1					13
#define SAT_THRO_2					14
#define SAT_RUDD_1					15
#define SAT_RUDD_2					16

#define satMAX 						7

volatile signed int SAT_channel[satMAX];


volatile unsigned char commandType;  			//which type of command are we retriving
volatile unsigned char commandNum;	  			//which subType or function of the commandType
volatile unsigned char I2CcmdType;
volatile unsigned char updateNum;	 			//in case of update we need to know what to update
volatile unsigned char updateCharVal;			//for now a char to store values 0-255
volatile unsigned char commandRetrived; 		//did we actualy retrive a command? 0 no 1 yes

unsigned char ygeActualAdress;
unsigned char ygeTargetAdress;

volatile unsigned char settingNumRequested; 	//which settingNum to be read 0-15 is a global as we change it during interupt
volatile unsigned char setCmd;					//which type of setupRequest we got

void checkSerialIn (unsigned char c);
void readSpektrum(unsigned char c);

void checkCmd(void);
void updateAcdRate(void);
void checkI2C(void);


#endif /*COMMAND_H*/
