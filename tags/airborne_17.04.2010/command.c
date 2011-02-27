/*
	file    command.c
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


#include "command.h"
#include "analog.h"
#include "init.h"
#include "viclowlevel.h"
#include "sys_config.h"
#include "i2c.h"
#include "settings.h"


volatile char *settingsStart, *settingsStop;	//pointers to packet currently transfered

volatile unsigned short	msgLen;
volatile unsigned char	ignorePacket;		//true when previous packet was not processed


void updateAcdRate(void) {
	if (updateCharVal > 255){
		updateCharVal = 255;
	} else if (updateCharVal < 0){
		updateCharVal = 0;
	}
	ADC_CLK_DIV = updateCharVal;
	disableIRQ();
	init_system();
	printDebug1();
}

void checkI2C(void) {

	if (I2C0Mode == I2CMODE_WRITEADDRESS){
		I2C0Start();
	}
	if (I2C0Mode == I2CMODE_READADDRESS){
		I2C0Start();
	}

}


void checkCmd(void) {
	switch (commandNum)
	{
		case 0:
			printDebug();
		break;
		case 1:
			printDebug1();
		break;
		case 2:
			printDebug2();
		break;
		case 3:
			printDebug3();
		break;
		case 4:
			printDebug4();
		break;
		case 5:
			printDebug5();
		break;
		case 6:
			printDebug6();
		break;
		case 7:
			printDebug7();
		break;
	}
}

volatile char CMD_CK_A, CMD_CK_B, msgLength;		//UBX checkSum bytes
char dynamicDebug;
char hit;

void checkSerialIn (unsigned char c){
	if (commandRetrived == 0) {
		switch (cmdState)
		{
			case CMD_EMPTY:
				if (c == CMD_SYNC_CHAR1)
				cmdState = CMD_SYNC1;
			break;
			case CMD_SYNC1:
			if (c == CMD_SYNC_CHAR2)
				cmdState = CMD_SYNC2;
			else if (c != CMD_SYNC_CHAR1)
				cmdState = CMD_EMPTY;
			break;
			case CMD_SYNC2:
			if (c == CMD_SYNC_DIV) {
				cmdState = CMD_DIV;
			} else if (c != CMD_SYNC_CHAR2) {
				cmdState = CMD_EMPTY;
			}
			break;
			case CMD_DIV:
			if (c == CMD_DEB_REQ) {
				commandType = 0;
				cmdState = CMD_REQ;
			} else if (c == CMD_SET_REQ) {
				commandType = 1;
				cmdState = CMD_SET;
			} else if (c == CMD_UPD_REQ) {
				commandType = 2;
				cmdState = UPD_REQ;
			} else if (c == CMD_RET_SET) {
				commandType = 3;
				cmdState = RET_SET;
			} else if (c == CMD_SET_YGE) {
				print_uart0("FCm0;was SET_YGE;00#");
				commandType = 4;
				cmdState = YGE_WRITE_TARGET_SRC;
			} else if (c == CMD_READ_YGE) {
				commandType = 4;
				cmdState = YGE_START_TARGET;
			} else if (c == CMD_START_YGE) {
				commandType = 4;
				cmdState = YGE_READ_ADR;
			} else if (c != CMD_SYNC_CHAR2) {
				cmdState = CMD_EMPTY;
			}
			break;
			case CMD_REQ:
			if (c == CMD_DEC_NUM0) {
				commandNum = 0;
			} else if (c == CMD_DEC_NUM1) {
				commandNum = 1;
			}else if (c == CMD_DEC_NUM2) {
				commandNum = 2;
			}else if (c == CMD_DEC_NUM3) {
				commandNum = 3;
			}else if (c == CMD_DEC_NUM4) {
				commandNum = 4;
			}else if (c == CMD_DEC_NUM5) {
				commandNum = 5;
			}else if (c == CMD_DEC_NUM6) {
				commandNum = 6;
			}else if (c == CMD_DEC_NUM7) {
				commandNum = 7;
			} else if (c == CMD_SYNC_DIV) {
				cmdState = SET_STOP;
			} else {
				cmdState = CMD_EMPTY;
			}
			break;
			case YGE_WRITE_TARGET_SRC:
				print_uart0("FCm0;was YGE_WRITE_TARGET_SRC;00#");
				ygeActualAdress = c;
				cmdState = YGE_WRITE_TARGET_TRG;
			break;
			case YGE_WRITE_TARGET_TRG:
				print_uart0("FCm0;was YGE_WRITE_TARGET_TRG;00#");
				ygeTargetAdress = c;
				updateYGE = 0;
				I2CcmdType = I2CMODE_WRITEADDRESS;
				I2C0Stop();
				cmdState = SET_STOP;
			break;
			case YGE_START_TARGET:
				ygeTargetAdress = c;
				I2C0State = 0;
				updateYGE = 0;
				I2C0Mode = I2CMODE_STARTUP_TARGET;
				cmdState = SET_STOP;
			break;
			case YGE_READ_ADR:
				ygeActualAdress = c;
				cmdState = SET_STOP;
			break;
			case YGE_READ_TARGET:
				ygeTargetAdress = c;
				I2C0State = 0;
				updateYGE = 0;
				I2C0Mode = I2CMODE_READADDRESS;
				cmdState = SET_STOP;
			break;
			case UPD_REQ:
			if (c == CMD_DEC_NUM0) {
				cmdState = RTR_UPD_VAL;
				updateNum = 0;
			}
			break;
			case RTR_UPD_VAL:
			if (updateNum == 0) {
				updateCharVal = c;
				cmdState = SET_STOP;
			}
			break;
			case RET_SET:
				settingNumRequested = c-48;
				print_uart0("FCm0;Settings Requested %d;00#",settingNumRequested);
				cmdState = SET_STOP;
			break;
			case CMD_SET:
				hit = 0;
				switch (c)
				{
					case CMD_DEC_NUM0:
						setCmd = 0;
						hit = 1;
						cmdState = SET_MSGLENGTH;
					break;
					case CMD_DEC_NUM1:
						setCmd = 1;
						hit = 1;
						cmdState = SET_STOP;
					break;
				}
				if (hit == 0) {
					cmdState = CMD_EMPTY;
				}
				break;
			case SET_MSGLENGTH:
				msgLength = c;
				if (msgLength > 255) {
					msgLength = 0;
				} else {
					settingsStart = (volatile char*)&setupCache;
					settingsStop = settingsStart+msgLength;
				}
				cmdState = SET_DIV;
			break;
			case SET_DIV:
				print_uart0("FCm0;Settings Retrieve started;00#");
				if (c == CMD_SYNC_DIV) {
					cmdState = SET_FILLING;
					CMD_CK_A = 0x01;
					CMD_CK_B = 0x01;
				} else {
					cmdState = CMD_EMPTY;
				}
			break;
			case SET_FILLING:
				if (settingsStart < settingsStop) {
					*settingsStart++ = c;
					CMD_CK_A += c;
					CMD_CK_B += CMD_CK_A;
				}
				if (msgLength-- == 1) {
					cmdState = CMD_DIV2;
				}
			break;
			case CMD_DIV2:
				print_uart0("FCm0;Settings Retrieve finished;00#");
			if (c == CMD_SYNC_DIV) {
				cmdState = CMD_CKA;
			}else {
				cmdState = CMD_EMPTY;
			}
			break;
			case CMD_CKA:
				print_uart0("FCm0;expect Checksum %2x got %2x;00#",CMD_CK_A,c);
			if (c == CMD_CK_A) {
				cmdState = CMD_CKB;
			} else {
				cmdState = CMD_EMPTY;
			}
			break;
			case CMD_CKB:
				print_uart0("FCm0;expect Checksum %2x got %2x;00#",CMD_CK_B,c);
			if (c == CMD_CK_B) {
				cmdState = SET_STOP;
			} else {
				cmdState = CMD_EMPTY;
				print_uart0("FCm0;Checksum wrong;00#");
			}
			break;
			case SET_STOP:
			if (c == CMD_STOP_CHAR) {
				print_uart0("FCm0;Command Retrieve OK;00#");
				commandRetrived = 1;
				cmdState = CMD_EMPTY;
			} else {
				commandRetrived = 0;
				cmdState = CMD_EMPTY;
			}
			break;
			default:
				cmdState = CMD_EMPTY;		//ready for next packet
		}
	}
}



