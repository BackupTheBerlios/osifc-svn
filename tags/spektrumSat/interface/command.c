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
#include "../arch/printf_p.h"
#include "../arch/settings.h"
#include "../arch/analog.h"
#include "../arch/init.h"
#include "../arch/viclowlevel.h"
#include "../arch/sys_config.h"
#include "../io/i2c.h"
#include "../nav/mm3parser.h"
#include "../arch/led.h"

volatile char *settingsStart, *settingsStop;	//pointers to packet currently transfered

volatile unsigned short	msgLen;


void updateAcdRate(void) {
	ADC_CLK_DIV = updateCharVal;
	disableIRQ();
	init_system();
	printDebug1();
}

void checkI2C(void) {

	if (I2C0Mode == I2CMODE_WRITEADDRESS){
		I2C0_Start();
	}
	if (I2C0Mode == I2CMODE_READADDRESS){
		I2C0_Start();
	}

}


void checkCmd(void) {
	switch (commandNum)
	{
		case CMD_PRINT_DEBUG:
			printDebug();
		break;
		case CMD_PRINT_DEBUG1:
			printDebug1();
		break;
		case CMD_PRINT_DEBUG2:
			printDebug2();
		break;
		case CMD_PRINT_DEBUG3:
			printDebug3();
		break;
		case CMD_PRINT_DEBUG4:
			printDebug4();
		break;
		case 5:

		break;
		case CMD_READ_CALIB_COMPASS:
			printCalibCompassValues();
		break;
		case CMD_WRITE_CALIB_COMPASS:

		break;
		// starts the MM3 calibration will later become start the compass calibration
		case CMD_START_CALIB_COMPASS:
			calib = 1;
			LED1_OFF;
			LED2_OFF;
			LED3_OFF;
			LED4_OFF;
		break;
		// stops the compass calibration
		case CMD_STOP_CALIB_COMPASS:
			calib = 0;
			ledTest();
			LED1_ON;
			LED2_OFF;
			LED3_OFF;
			LED4_OFF;
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
				commandNum = CMD_PRINT_DEBUG;
			} else if (c == CMD_DEC_NUM1) {
				commandNum = CMD_PRINT_DEBUG1;
			} else if (c == CMD_DEC_NUM2) {
				commandNum = CMD_PRINT_DEBUG2;
			} else if (c == CMD_DEC_NUM3) {
				commandNum = CMD_PRINT_DEBUG3;
			} else if (c == CMD_DEC_NUM4) {
				commandNum = CMD_PRINT_DEBUG4;
			} else if (c == CMD_DEC_NUM5) {
				commandNum = 5;
			} else if (c == CMD_DEC_NUM6) {
				commandNum = CMD_READ_CALIB_COMPASS;
			} else if (c == CMD_DEC_NUM7) {
				commandNum = CMD_WRITE_CALIB_COMPASS;
			} else if (c == CMD_DEC_NUM8) {
				commandNum = CMD_START_CALIB_COMPASS;
			} else if (c == CMD_DEC_NUM9) {
				commandNum = CMD_STOP_CALIB_COMPASS;
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
				I2C0_Stop();
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
				settingsStart = (volatile char*)&setupCache;
				settingsStop = settingsStart+msgLength;
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

/*
 *
 * GND 		= Black
 * Signal 	= Grey goes to RXD1
 * VCC3.3   = Orange
 * Speed at 115000 BPS 8N1
 * Signal comes as two Byte Data
 * Sync AILE FLAPS GEAR ELEV AUX2 THRO RUDD
 * Sync is 0301
 * There seems to be no checksum so we have to thrust the data anyways
 *
 *
 * There is some conjecture here but we suspect this is pretty close to what happens:

At power up the remote receiver [SPM9545] samples the data line, if it is high it waits for a bind request.

When in bind mode the main receiver performs the following actions:
It sets that data line to the remote receiver to high, waits 100ms and then send the following:

120µS Low
120µS High
120µS Low
120µS High
120µS Low
120µS High
120µS Low
then remains High
 */

char tempi;
char satTemp;
void readSpektrum(unsigned char c)
{
	if (tempi++ > 4)
		tempi = 1;
	//return;
	led_switch(tempi);
	//led_switch(2);

	//print_uart0("%2x",c);

	switch (satState)
	{
		case SAT_SYNC_1:
			if (c == SAT_START_1)
			satState = SAT_SYNC_2;
		break;
		case SAT_SYNC_2:
			if (c == SAT_START_2)
			satState = SAT_AILE_1;
		break;
		case SAT_AILE_1:
			satTemp = c;
			satState = SAT_AILE_2;
		break;
		case SAT_AILE_2:
			SAT_channel[0] = c;
			SAT_channel[0] |= (satTemp << 8);
			SAT_channel[0] -= 1024;
			SAT_channel[0] /= 3;
			SAT_channel[0] -= 170;
			PWM_channel[PWM_R] = SAT_channel[0];
			satState = SAT_FLAPS_1;
		break;
		case SAT_FLAPS_1:
			satTemp = c;
			satState = SAT_FLAPS_2;
		break;
		case SAT_FLAPS_2:
			SAT_channel[1] = c;
			SAT_channel[1] |= (satTemp << 8);
			SAT_channel[1] -= 5120;
			SAT_channel[1] /= 3;
			SAT_channel[1] -= 170;
			satState = SAT_GEAR_1;
		break;
		case SAT_GEAR_1:
			satTemp = c;
			satState = SAT_GEAR_2;
		break;
		case SAT_GEAR_2:
			SAT_channel[2] = c;
			SAT_channel[2] |= (satTemp << 8);
			SAT_channel[2] -= 4096;
			SAT_channel[2] /= 3;
			SAT_channel[2] -= 170;
			satState = SAT_ELEV_1;
		break;
		case SAT_ELEV_1:
			satTemp = c;
			satState = SAT_ELEV_2;
		break;
		case SAT_ELEV_2:
			SAT_channel[3] = c;
			SAT_channel[3] |= (satTemp << 8);
			SAT_channel[3] -= 2048;
			SAT_channel[3] /= 3;
			SAT_channel[3] -= 170;
			PWM_channel[PWM_N] = SAT_channel[3];
			satState = SAT_AUX2_1;
		break;
		case SAT_AUX2_1:
			satTemp = c;
			satState = SAT_AUX2_2;
		break;
		case SAT_AUX2_2:
			SAT_channel[4] = c;
			SAT_channel[4] = (satTemp << 8);
			SAT_channel[4] -= 6144;
			SAT_channel[4] /= 3;
			SAT_channel[4] -= 170;
			satState = SAT_THRO_1;
		break;
		case SAT_THRO_1:
			satTemp = c;
			satState = SAT_THRO_2;
		break;
		case SAT_THRO_2:
			SAT_channel[5] = c;
			SAT_channel[5] |= (satTemp << 8);
			SAT_channel[5] /= 3;
			SAT_channel[5] -= 170;
			PWM_channel[PWM_THROTTLE] = SAT_channel[5];
			satState = SAT_RUDD_1;
		break;
		case SAT_RUDD_1:
			satTemp = c;
			satState = SAT_RUDD_2;
		break;
		case SAT_RUDD_2:
			SAT_channel[6] = c;
			SAT_channel[6] |= (satTemp << 8);
			SAT_channel[6] -= 3072;
			SAT_channel[6] /= 3;
			SAT_channel[6] -= 170;
			PWM_channel[PWM_G] = SAT_channel[6];
			satState = SAT_SYNC_1;

			print_uart0("FCm0;satData %d %d %d %d %d %d %d  ;00#"
					,SAT_channel[0]
					,SAT_channel[1]
					,SAT_channel[2]
					,SAT_channel[3]
					,SAT_channel[4]
					,SAT_channel[5]
					,SAT_channel[6]
			);
		break;
		default:
			satState = SAT_SYNC_1;		//ready for next packet

	}
}




