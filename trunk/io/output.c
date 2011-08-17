/*
	file    output.c
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



#include "output.h"
#include <stdlib.h>
#include <string.h>

#include "engines.h"
#include "i2c.h"
#include "uart.h"
#include "compass.h"
#include "pwmin.h"

#include "../arch/analog.h"
#include "../arch/clock-arch.h"
#include "../arch/printf_p.h"
#include "../arch/lpc23xx.h"
#include "../arch/settings.h"
#include "../arch/iapcore.h"
#include "../arch/sys_config.h"


#include "../math/pidfc.h"
#include "../fc/osifc.h"
#include "../interface/command.h"

#include "../nav/mm3parser.h"
#include "../nav/NMEAparser.h"

//this is where all the print out is generated. by now this is kind of in constant change.
//I will stabilize this soon once I learned which makes most sense. you are welcomed to help
//toDo the plan is to have kind of a general stable output and also output which you change on demand
//also I plan to have the firmware tell the base station which data to expect this should make
//base station more robust against dynamic output changes in the wild, again feel free to rush in any
//ready to use solution

const float ubatDivider = 5.473;

void checkIapResult(int resultValue)
{

	//print back a status of operation
	//toDo make this numbers and interpreted by base station tool

	print_uart0("FCm0;size %d;00#",sizeof(setupStore));
	if (resultValue==CMD_SUCCESS)
		print_uart0("FCm0;CMD_SUCCESS;00#");
	else if(resultValue==BUSY)
		print_uart0("FCm0;BUSY;00#");
	else if(resultValue==SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION)
		print_uart0("FCm0;SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION;00#");
	else if(resultValue==SRC_ADDR_ERROR)
		print_uart0("FCm0;SRC_ADDR_ERROR;00#");
	else if(resultValue==DST_ADDR_ERROR)
		print_uart0("FCm0;DST_ADDR_ERROR;00#");
	else if(resultValue==SRC_ADDR_NOT_MAPPED)
		print_uart0("FCm0;SRC_ADDR_NOT_MAPPED;00#");
	else if(resultValue==DST_ADDR_NOT_MAPPED)
		print_uart0("FCm0;DST_ADDR_NOT_MAPPED;00#");
	else if(resultValue==COUNT_ERROR)
		print_uart0("FCm0;COUNT_ERROR;00#");
	else if(resultValue==SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION)
		print_uart0("FCm0;Erase Sector SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION;00#");
	else if(resultValue==INVALID_SECTOR)
		print_uart0("FCm0;Erase Sector INVALID_SECTOR;00#");

}

void checkI2CResult(int resultValue, int num)
{

	//print back a status of operation
	//toDo make this numbers and interpreted by base station tool
	print_uart0("FCm0;status %X;00#",resultValue);
	if (resultValue==I2C_STARTUP_SEND)
		print_uart0("FCm0;A START condition has been transmitted");
	else if(resultValue==I2C_REPEADET_STARTUP_SEND)
		print_uart0("FCm0;A repeated START condition has been transmitted");
	else if(resultValue==I2C_SLAW_SEND_ACK)
		print_uart0("FCm0;SLA+W has been transmitted; ACK has been received");
	else if(resultValue==I2C_SLAW_SEND_NO_ACK)
		print_uart0("FCm0;SLA+W has been transmitted; NOT ACK has been received");
	else if(resultValue==I2C_DATA_SEND_ACK)
		print_uart0("FCm0;Data byte in I2DAT has been transmitted; ACK has been received");
	else if(resultValue==I2C_DATA_SEND_NO_ACK)
		print_uart0("FCm0;Data byte in I2DAT has been transmitted; NOT ACK has been received");
	else if(resultValue==I2C_ARBITRATION_LOST)
		print_uart0("FCm0;Arbitration lost in SLA+R/W or Data bytes");
	else if(resultValue==I2C_SLAR_SEND_ACK)
		print_uart0("FCm0;SLA+R has been transmitted; ACK has been received");
	else if(resultValue==I2C_SLAR_SEND_NO_ACK)
		print_uart0("FCm0;SLA+R has been transmitted; NOT ACK has been received");
	else if(resultValue==I2C_DATA_RECEIVED_SEND_ACK)
		print_uart0("FCm0;Data byte has been received; ACK has been returned");
	else if(resultValue==I2C_DATA_RECEIVED_SEND_NO_ACK)
		print_uart0("FCm0;Data byte has been received; NOT ACK has been returned");

	print_uart0(" %d;00#",num);
}

void printDot(void)
{
	print_uart0(".");
}

int GetPartID(void)
{
	IAP iap_entry = (IAP) IAP_LOCATION;
	command[0] = ReadPartID;
	iap_entry (command, result);

	if (result[0]==CMD_SUCCESS)
	{
		print_uart0("FCm0;Get Part ID: CMD_SUCCESS;00#");
		print_uart0("FCm0;0x%.8X;00#",result[1]);
	}
	return result[0];
}


int GetBootCodeVersion(void)
{
	IAP iap_entry = (IAP) IAP_LOCATION;
	command[0] = ReadBootCodeVersion;
	iap_entry (command, result);

	if (result[0]==CMD_SUCCESS)
	{
		print_uart0("FCm0;Get Boot Code Version: CMD_SUCCESS;00#");
		print_uart0("FCm0;0x%.8X;00#",result[1]);
	}

	return result[0];
}

void printLoadSetting(char setNum)
{
	print_uart0("..loading setting %d..",setNum);
}


// Debug data. Its up to you how to use it


void printDebug(void)
{

	RTCTime current_time;
	current_time = RTC_Get_Time();

	print_uart0("FCd0;");
	print_uart0("%d;%d;%d;",
		current_time.RTC_Sec,
		current_time.RTC_Min,
		refreshCount
	);
	print_uart0(";00#");
}

void printDebug1(void)
{

	RTCTime current_time;
	current_time = RTC_Get_Time();

	print_uart0("FCd1;");
	print_uart0("%d;%d;%d;",
		(int)current_time.RTC_Sec,
		(int)current_time.RTC_Min,
		(int)refreshCount / 10
	);

	print_uart0("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
		(int)PWMEngOut[0],
		(int)PWMEngOut[1],
		(int)PWMEngOut[2],
		(int)PWMEngOut[3],
		(int)(ADC_runtime[ADC_NICK]-ADC_drift[ADC_NICK]),
		(int)(ADC_runtime[ADC_ROLL]-ADC_drift[ADC_ROLL]),
		(int)(ADC_runtime[ADC_PITCH]+ADC_drift[ADC_PITCH]),
		(int)ADC_runtime[ADC_ACCX],
		(int)ADC_runtime[ADC_ACCY],
		(int)ADC_runtime[ADC_ACCZ]);


	print_uart0("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;",
		(int)ADC_runtime[AIRPRESSURE],
		(int)MM3_runtime.X_axis,
		(int)MM3_runtime.Y_axis,
		(int)MM3_runtime.Z_axis,
		(signed int)PWM_channel[0],
		(signed int)PWM_channel[1],
		(signed int)PWM_channel[2],
		(signed int)PWM_channel[3],
		(signed int)PWM_channel[4],
		(signed int)PWM_channel[5]);

	print_uart0("%d;%d",
		(signed int)PWM_channel[6],
		(signed int)(ADC_raw[UBAT] / ubatDivider)
	);

	print_uart0(";00#");
}



void printDebug2(void)
{
	RTCTime current_time;
	current_time = RTC_Get_Time();

	print_uart0("FCd2;");
	print_uart0("%d;%d;%d;",
		(int)current_time.RTC_Sec,
		(int)current_time.RTC_Min,
		(int)refreshCount / 10
	);
	print_uart0(";00#");
}
void printDebug3(void)
{
	RTCTime current_time;
	current_time = RTC_Get_Time();

	print_uart0("FCd3;");
	print_uart0("%d;%d;%d;",
		(int)current_time.RTC_Sec,
		(int)current_time.RTC_Min,
		(int)refreshCount / 10
	);
	print_uart0(";00#");
}
void printDebug4(void)
{
	RTCTime current_time;
	current_time = RTC_Get_Time();

	print_uart0("FCd4;");
	print_uart0("%d;%d;%d;",
		(int)current_time.RTC_Sec,
		(int)current_time.RTC_Min,
		(int)refreshCount / 10
	);
	print_uart0(";00#");
}


// be carefull to change the below
// this is the Telemetrie data

void printTelemetrie(void)
{

	//return;

	RTCTime current_time;
	current_time = RTC_Get_Time();

	print_uart0("FCt0;");
	print_uart0("%d;%d;%d;",
		(int)current_time.RTC_Min,
		(int)current_time.RTC_Sec,
		(int)lastCycleCount / 10);


	print_uart0("%d;%d;%d;%d;%d;%d;%d;%d;",
		//( AD0DR0 >> 6 ) & 0x3FF,
		(int)((float)ADC_runtime[ADC_NICK]-ADC_drift[ADC_NICK]),
		(int)((float)ADC_runtime[ADC_ROLL]-ADC_drift[ADC_ROLL]),
		(int)((float)ADC_runtime[ADC_PITCH]+ADC_drift[ADC_PITCH]),
		//( AD0DR3 >> 6 ) & 0x3FF,
		(signed int)ADC_runtime[ADC_ACCX] - ADC_standStill[ADC_ACCX],
		(signed int)ADC_runtime[ADC_ACCY] - ADC_standStill[ADC_ACCY],
		(int)ADC_runtime[ADC_ACCZ],
		(int)ADC_runtime[AIRPRESSURE],
		(int)(ADC_runtime[UBAT]/ubatDivider));


	print_uart0("%d;%d;%d;%d;",
		(int)PWMEngOut[0],
		(int)PWMEngOut[1],
		(int)PWMEngOut[2],
		(int)PWMEngOut[3]);

	print_uart0("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
		(signed int)MM3_runtime.heading,
		(signed int)MM3_runtime.X_axis,
		(signed int)MM3_runtime.Y_axis,
		(signed int)MM3_runtime.Z_axis,
		(int)NMEAGPGGA.lat*10000,
		(int)NMEAGPGGA.lon*10000,
		(int)PWM_channel[PWM_N],
		(int)PWM_channel[PWM_G],
		(int)PWM_channel[PWM_POTI1],
		(int)PWM_channel[PWM_POTI2],
		//(signed int)hmcXvalue,//(int)PWM_channel[PWM_POTI2]+120,
		(int)PWM_channel[PWM_POTI3]);
	print_uart0(";00#");

}

// print the compass values

void printCalibCompassValues(void)
{


	print_uart0("FCc0;");

	print_uart0("%d;%d;%d;%d;",
	sysSetup.MM3.X_MIN,
	sysSetup.MM3.X_MAX,
	sysSetup.MM3.X_range,
	sysSetup.MM3.X_off);

	print_uart0("%d;%d;%d;%d;",
	sysSetup.MM3.Y_MIN,
	sysSetup.MM3.Y_MAX,
	sysSetup.MM3.Y_range,
	sysSetup.MM3.Y_off);

	print_uart0("%d;%d;%d;%d",
	sysSetup.MM3.Z_MIN,
	sysSetup.MM3.Z_MAX,
	sysSetup.MM3.Z_range,
	sysSetup.MM3.Z_off);

	print_uart0(";00#");


}


// to send the settings to the basestation tool.
// do not play here unless you know what you do

void printSettings(void)
{

	print_uart0("FCse;");

	print_uart0("%d;",
			(char)setupCache.settingNum);
	print_uart0("%d;%d;%d;%d;",
			(char)setupCache.barOn,
			(char)setupCache.barChan,
			(char)setupCache.barOffset,
			(char)setupCache.compOn);

	print_uart0("%d;%d;%d;",
			(char)setupCache.compBias,
			(char)setupCache.compForce,
			(char)setupCache.compType);

	print_uart0("%d;%d;",
			(char)setupCache.gpsOn,
			(char)setupCache.nickServoOn);

	print_uart0("%d;",
			(char)setupCache.nickServoChan);
	print_uart0("%d;%d;%d;",
			(char)setupCache.nickServoInvert,
			(char)setupCache.nickServoPos,
			(char)setupCache.nickServoForce);
	print_uart0("%d;%d;",
			(char)setupCache.nickServoMin,
			(char)setupCache.nickServoMax);

	print_uart0("%d;%d;",
			(char)setupCache.rollServoOn,
			(char)setupCache.rollServoChan);
	print_uart0("%d;%d;%d;",
			(char)setupCache.rollServoInvert,
			(char)setupCache.rollServoPos,
			(char)setupCache.rollServoForce);
	print_uart0("%d;%d;",
			(char)setupCache.rollServoMin,
			(char)setupCache.rollServoMax);

	print_uart0("%d;%d;",
			(char)setupCache.pid_PitchStickFact,
			(char)setupCache.pid_throttleOffset);
	print_uart0("%d;",
			(char)setupCache.sysGasMin);

	print_uart0("%d;%d;%d;",
			(char)setupCache.sysEmergencyGas,
			(char)setupCache.sysEmergencyGasDuration,
			(char)setupCache.sysLowVoltage);
	print_uart0("%d;%d;%d;%d;%d;",
			(char)setupCache.sysMainDirection,
			(char)setupCache.sysGasMax,
			(char)setupCache.ReceiverType,
			(char)setupCache.PWMMode,
			(char)setupCache.BTMode);

	print_uart0("%d;%d;%d;",
			(char)setupCache.ADCModeNick,
			(char)setupCache.ADCModeRoll,
			(char)setupCache.ADCModePitch);

	print_uart0("%d;%d;%d;",
			(char)setupCache.ADCDriftNick,
			(char)setupCache.ADCDriftRoll,
			(char)setupCache.ADCDriftPitch);

	print_uart0("%d;%d;%d;",
			(char)setupCache.calcMode,
			(char)setupCache.pid_X_AccX_Fact,
			(char)setupCache.pid_X_GyroSumFact);
	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_X_P,
			(char)setupCache.pid_X_D,
			(char)setupCache.pid_X_I);
	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_X_ACC_P,
			(char)setupCache.pid_X_ACC_D,
			(char)setupCache.pid_X_ACC_I);

	print_uart0("%d;%d;%d;%d;",
			(char)setupCache.pid_X_PitchSumFact,
			(char)setupCache.pid_X_SumFact,
			(char)setupCache.X_sensorBias,
			(char)setupCache.X_sensorBiasNeg);

	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_Y_AccY_Fact,
			(char)setupCache.pid_Y_GyroSumFact,
			(char)setupCache.pid_Y_P);
	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_Y_D,
			(char)setupCache.pid_Y_I,
			(char)setupCache.pid_Y_ACC_P);
	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_Y_ACC_D,
			(char)setupCache.pid_Y_ACC_I,
			(char)setupCache.pid_Y_PitchSumFact);

	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_Y_SumFact,
			(char)setupCache.Y_sensorBias,
			(char)setupCache.Y_sensorBiasNeg);

	print_uart0("%d;%d;%d;%d;%d;",
			(char)setupCache.pid_GyroPitchFact,
			(char)setupCache.pid_NickStickFact,
			(char)setupCache.pid_RollStickFact,
			(char)setupCache.pid_HeadingHold,
			(char)setupCache.pid_PitchP);

	print_uart0("%d;%d;%d;%d;%d;",
			(char)setupCache.pid_PitchI,
			(char)setupCache.pid_PitchD,
			(char)setupCache.PitchSensorBias,
			(char)setupCache.PitchSensorBiasNeg,
			(char)setupCache.pid_PitchThrottleFact);

	print_uart0("%d;%d;",
			(char)setupCache.pid_X_GyroACCFactMin,
			(char)setupCache.pid_X_GyroACCFactMax);

	print_uart0("%d;%d;",
			(char)setupCache.pid_X_IntegralMin,
			(char)setupCache.pid_X_IntegralMax);

	print_uart0("%d;%d;",
			(char)setupCache.pid_Y_GyroACCFactMin,
			(char)setupCache.pid_Y_GyroACCFactMax);

	print_uart0("%d;%d;",
			(char)setupCache.pid_Y_IntegralMin,
			(char)setupCache.pid_Y_IntegralMax);

	print_uart0("%d;%d;",
			(char)setupCache.pid_Pitch_IntegralMin,
			(char)setupCache.pid_Pitch_IntegralMax);

	print_uart0("%d;%d;",
			(char)setupCache.escType,
			(char)setupCache.escMax);

	print_uart0("%d;%d;",
			(char)setupCache.escBaseAdr,
			(char)setupCache.escAdrHop);

	print_uart0("%d;%d;",
			(char)setupCache.calcCycle,
			(char)setupCache.telemetrieCycle);

	print_uart0("%d;%d;",
			(char)setupCache.componentCycle,
			(char)setupCache.AdcClockDiv);

	print_uart0("%d;%d;%d;",
			(char)setupCache.MaxValue,
			(char)setupCache.MinValue,
			(char)setupCache.MaxMultichannel);
	print_uart0("%d;%d;",
			(char)setupCache.mcOffset,
			(char)setupCache.mcOffset2);

	print_uart0("%d;",
			(char)setupCache.scOffsetBase);


	print_uart0("%d;%d;",
			(char)setupCache.HMC5843_Mode,
			(char)setupCache.HMC5843_Bias);
	print_uart0("%d;%d;%d;",
			(char)setupCache.HMC5843_Delay,
			(char)setupCache.HMC5843_Gain,
			(char)setupCache.HMC5843_Rate);


	print_uart0("%d;%d;",
			(char)setupCache.chan[0],
			(char)setupCache.chan[1]);
	print_uart0("%d;%d;%d;",
			(char)setupCache.chan[2],
			(char)setupCache.chan[3],
			(char)setupCache.chan[4]);
	print_uart0("%d;%d;",
			(char)setupCache.chan[5],
			(char)setupCache.chan[6]);
	print_uart0("%d;%d;%d;",
			(char)setupCache.chan[7],
			(char)setupCache.chan[8],
			(char)setupCache.chan[9]);
	print_uart0("%d;%d;",
			(char)setupCache.chan[10],
			(char)setupCache.chan[11]);
	print_uart0("%d;%d;%d;%d;",
			(char)setupCache.mpOffset[0],
			(char)setupCache.mpOffset[1],
			(char)setupCache.mpOffset[2],
			(char)setupCache.mpOffset[3]);
	print_uart0("%d;%d;%d;%d;%d;",
			(char)setupCache.mpOffset[4],
			(char)setupCache.mpOffset[5],
			(char)setupCache.mpOffset[6],
			(char)setupCache.mpOffset[7],
			(char)setupCache.mpOffset[8]);


	print_uart0("%d;%d;%d;%d;%d;",
			(char)setupCache.userSetting[0],
			(char)setupCache.userSetting[1],
			(char)setupCache.userSetting[2],
			(char)setupCache.userSetting[3],
			(char)setupCache.userSetting[4]);
	print_uart0("%d;%d;%d;%d;%d;",
			(char)setupCache.userSetting[5],
			(char)setupCache.userSetting[6],
			(char)setupCache.userSetting[7],
			(char)setupCache.userSetting[8],
			(char)setupCache.userSetting[9]);

	print_uart0("%d;%d;%d;%d;%d;",
			(char)setupCache.userSetting[10],
			(char)setupCache.userSetting[11],
			(char)setupCache.userSetting[12],
			(char)setupCache.userSetting[13],
			(char)setupCache.userSetting[14]);
	print_uart0("%d;%d;%d;%d;%d;%d",
			(char)setupCache.userSetting[15],
			(char)setupCache.userSetting[16],
			(char)setupCache.userSetting[17],
			(char)setupCache.userSetting[18],
			(char)setupCache.userSetting[19],
			(char)setupCache.userSetting[20]);

	print_uart0(";00#");

}


