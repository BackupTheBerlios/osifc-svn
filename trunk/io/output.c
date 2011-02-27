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

#include "arch/analog.h"
#include "arch/clock-arch.h"
#include "arch/printf_p.h"
#include "arch/lpc23xx.h"
#include "arch/settings.h"
#include "arch/iapcore.h"

#include "io/i2c.h"
#include "io/uart.h"
#include "io/compass.h"
#include "io/pwmin.h"

#include "math/pidfc.h"
#include "fc/osifc.h"
#include "interface/command.h"

#include "nav/mm3parser.h"
#include "nav/NMEAparser.h"

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

int GetPartID ()
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


int GetBootCodeVersion ()
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


void printDebug1(void)
{
	//return;
	RTCTime current_time;

	current_time = RTCGetTime();

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
			//print_uart0(" \r\n");
}

void printTelemetrie(void)
{
	RTCTime current_time;
	current_time = RTCGetTime();

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
			(int)ADC_runtime[ADC_ACCX],
			(int)ADC_runtime[ADC_ACCY],
			(int)ADC_runtime[ADC_ACCZ],
			(int)ADC_runtime[AIRPRESSURE],
			(int)(float)(ADC_runtime[UBAT]/ubatDivider));

		print_uart0("%d;%d;%d;%d;",
			(int)PWMEngOut[0],
			(int)PWMEngOut[1],
			(int)PWMEngOut[2],
			(int)PWMEngOut[3]);

		print_uart0("%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
			(int)MM3_runtime.heading,
			(int)MM3_runtime.X_axis,
			(int)MM3_runtime.Y_axis,
			(int)MM3_runtime.Z_axis,
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



void printDebug(void)
{
	//return;
	RTCTime current_time;

		current_time = RTCGetTime();

		print_uart0("FCd0;");

		print_uart0("%d;%d;%d;",
			current_time.RTC_Sec,
			current_time.RTC_Min,
			refreshCount
		);
		print_uart0(";00#");
}


void printDebug2(void)
{
}

void printSettings(void)
{

	print_uart0("FCse;");

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




	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_PitchStickFact,
			(char)setupCache.pid_throttleOffset,
			(char)setupCache.pd_PitchStickFact);
	print_uart0("%d;%d;%d;",
			(char)setupCache.pd_throttleOffset,
			(char)setupCache.sysGasMin,
			(char)setupCache.sysRcGasMax);

	print_uart0("%d;%d;%d;",
			(char)setupCache.sysEmergencyGas,
			(char)setupCache.sysEmergencyGasDuration,
			(char)setupCache.sysLowVoltage);
	print_uart0("%d;%d;%d;%d;",
			(char)setupCache.sysMainDirection,
			(char)setupCache.sysGasMax,
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

	print_uart0("%d;%d;%d;",
			(char)setupCache.pid_GyroPitchFact,
			(char)setupCache.pid_StickFact,
			(char)setupCache.pid_PitchP);

	print_uart0("%d;%d;%d;%d;",
			(char)setupCache.pid_PitchI,
			(char)setupCache.pid_PitchD,
			(char)setupCache.PitchSensorBias,
			(char)setupCache.PitchSensorBiasNeg);

	print_uart0("%d;%d;%d;",
			(char)setupCache.pd_X_P_Fact,
			(char)setupCache.pd_X_D_Fact,
			(char)setupCache.pd_Y_P_Fact);
	print_uart0("%d;%d;",
			(char)setupCache.pd_Y_D_Fact,
			(char)setupCache.pd_PitchP);

	print_uart0("%d;%d;%d;",
			(char)setupCache.pd_PitchD,
			(char)setupCache.pd_X_AccX_Fact,
			(char)setupCache.pd_Y_AccY_Fact);

	print_uart0("%d;%d;%d;",
			(char)setupCache.pd_X_GyroSumFact,
			(char)setupCache.pd_X_PitchSumFact,
			(char)setupCache.pd_Y_GyroSumFact);

	print_uart0("%d;%d;%d;%d;",
			(char)setupCache.pd_Y_PitchSumFact,
			(char)setupCache.pd_GyroPitchFact,
			(char)setupCache.pd_StickFact,
			(char)setupCache.settingNum);

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

void printDebug3(void)
{

	print_uart0("FCd3;");
	print_uart0("%X;%X;%X;%X;%X;%X;%X;%X;%X;%X;",
				setupCache.chan[0],
				setupCache.chan[1],
				setupCache.chan[2],
				setupCache.chan[3],
				setupCache.chan[4],
				setupCache.chan[5],
				setupCache.chan[6],
				setupCache.chan[7],
				setupCache.chan[8],
				setupCache.chan[9]);

		print_uart0("%X;%X;%X;%X;%X;%X;%X;%X;",
				setupCache.chan[10],
				setupCache.chan[11],
				setupCache.barOn,
				setupCache.compOn,
				setupCache.gpsOn,
				setupCache.nickServoOn,
				setupCache.rollServoOn);

	print_uart0("%X;%X;%X;%X;%X;%X;%X;%X;%X;%X;%X;%X;",
				setupCache.pid_PitchStickFact,
				setupCache.pid_throttleOffset,
				setupCache.pd_PitchStickFact,
				setupCache.pd_throttleOffset,
				setupCache.sysGasMin,
				setupCache.sysRcGasMax,
				setupCache.sysEmergencyGas,
				setupCache.sysEmergencyGasDuration,
				setupCache.sysLowVoltage,
				setupCache.sysMainDirection,
				setupCache.sysGasMax,
				setupCache.PWMMode);

	print_uart0("%X;%X;%X;%X;%X;%X;%X;%X;",
				setupCache.calcMode,
				setupCache.pid_X_AccX_Fact,
				setupCache.pid_X_GyroSumFact,
				setupCache.pid_X_P,
				setupCache.pid_X_D,
				setupCache.pid_X_I,
				setupCache.pid_X_PitchSumFact,
				setupCache.pid_X_SumFact);



	print_uart0("%X;%X;%X;%X;%X;%X;%X;",
				setupCache.pid_Y_AccY_Fact,
				setupCache.pid_Y_GyroSumFact,
				setupCache.pid_Y_P,
				setupCache.pid_Y_D,
				setupCache.pid_Y_I,
				setupCache.pid_Y_PitchSumFact,
				setupCache.pid_Y_SumFact);

	print_uart0("%X;%X;%X;%X;%X;",
				setupCache.pid_GyroPitchFact,
				setupCache.pid_StickFact,
				setupCache.pid_PitchP,
				setupCache.pid_PitchI,
				setupCache.pid_PitchD);

	print_uart0("%X;%X;%X;%X;%X;%X;%X;%X;%X;%X;%X;",
				setupCache.pd_X_P_Fact,
				setupCache.pd_X_D_Fact,
				setupCache.pd_Y_P_Fact,
				setupCache.pd_Y_D_Fact,
				setupCache.pd_PitchP,
				setupCache.pd_PitchD,
				setupCache.pd_X_AccX_Fact,
				setupCache.pd_Y_AccY_Fact,
				setupCache.pd_X_GyroSumFact,
				setupCache.pd_X_PitchSumFact,
				setupCache.pd_Y_GyroSumFact);

	print_uart0("%X;%X;%X;%X;",
				setupCache.pd_Y_PitchSumFact,
				setupCache.pd_GyroPitchFact,
				setupCache.pd_StickFact,
				setupCache.settingNum);

	print_uart0("%X;%X;%X;%X;%X;%X;%X;%X;",
				setupCache.pid_X_GyroACCFactMin,
				setupCache.pid_X_GyroACCFactMax,
				setupCache.pid_X_IntegralMin,
				setupCache.pid_X_IntegralMax,
				setupCache.pid_Y_GyroACCFactMin,
				setupCache.pid_Y_GyroACCFactMax,
				setupCache.pid_Y_IntegralMin,
				setupCache.pid_Y_IntegralMax);

	print_uart0("%X;%X;",
				setupCache.pid_Pitch_IntegralMin,
				setupCache.pid_Pitch_IntegralMax);

	print_uart0("%X;%X;%X;%X;",
				setupCache.escType,
				setupCache.escMax,
				setupCache.escBaseAdr,
				setupCache.escAdrHop);

	print_uart0("%X;%X;%X;%X;",
				setupCache.calcCycle,
				setupCache.telemetrieCycle,
				setupCache.componentCycle,
				setupCache.AdcClockDiv);

	print_uart0("%X;%X;%X;%X;%X;",
				setupCache.MaxValue,
				setupCache.MinValue,
				setupCache.MaxMultichannel,
				setupCache.mcOffset,
				setupCache.mcOffset2);
	print_uart0("%X;%X;%X;%X;%X;",
				setupCache.scOffsetBase,
				setupCache.mpOffset[0],
				setupCache.mpOffset[1],
				setupCache.mpOffset[2],
				setupCache.mpOffset[3]);

	print_uart0("%X;%X;%X;%X;%X;",
				setupCache.mpOffset[4],
				setupCache.mpOffset[5],
				setupCache.mpOffset[6],
				setupCache.mpOffset[7],
				setupCache.mpOffset[8]);

	print_uart0("%X;%X;%X;%X;%X;",
				setupCache.userSetting[0],
				setupCache.userSetting[1],
				setupCache.userSetting[2],
				setupCache.userSetting[3],
				setupCache.userSetting[4]);
	print_uart0("%X;%X;%X;%X;%X;",
				setupCache.userSetting[5],
				setupCache.userSetting[6],
				setupCache.userSetting[7],
				setupCache.userSetting[8],
				setupCache.userSetting[9]);


	print_uart0("%X;%X;%X;%X;%X;",
				setupCache.userSetting[10],
				setupCache.userSetting[11],
				setupCache.userSetting[12],
				setupCache.userSetting[13],
				setupCache.userSetting[14]);
	print_uart0("%X;%X;%X;%X;%X;%X",
				setupCache.userSetting[15],
				setupCache.userSetting[16],
				setupCache.userSetting[17],
				setupCache.userSetting[18],
				setupCache.userSetting[19],
				setupCache.userSetting[20]);


	print_uart0(";00#");
}


void printDebug4(void)
{

	print_uart0("FCd5;");
	print_uart0("%d;%d;%d;%d;%d;",
				(int)fcSetup.chan[0],
				(int)fcSetup.chan[1],
				(int)fcSetup.chan[2],
				(int)fcSetup.chan[3],
				(int)fcSetup.chan[4]);

	print_uart0("%d;%d;%d;%d;%d;",
				(int)fcSetup.chan[5],
				(int)fcSetup.chan[6],
				(int)fcSetup.chan[7],
				(int)fcSetup.chan[8],
				(int)fcSetup.chan[9]);

	print_uart0("%d;%d;%d;%d;",
				(int)fcSetup.chan[10],
				(int)fcSetup.chan[11],
				(int)fcSetup.components[0],
				(int)fcSetup.components[1]);

	print_uart0("%d;%d;%d;",
				(int)fcSetup.components[2],
				(int)fcSetup.nickServoOn,
				(int)fcSetup.rollServoOn);

	print_uart0("%d;%d;%d;%d;",
				(int)fcSetup.pid_PitchStickFact,
				(int)fcSetup.pid_throttleOffset,
				(int)fcSetup.pd_PitchStickFact,
				(int)fcSetup.pd_throttleOffset);

	print_uart0("%d;%d;",
				(int)fcSetup.sysGasMin,
				(int)fcSetup.sysRcGasMax);

	print_uart0("%d;%d;%d;%d;",
				(int)fcSetup.sysEmergencyGas,
				(int)fcSetup.sysEmergencyGasDuration,
				(int)fcSetup.sysLowVoltage,
				(int)fcSetup.sysMainDirection);

	print_uart0("%d;%d;%d;",
				(int)fcSetup.serialControl,
				(int)fcSetup.sysGasMax,
				(int)fcSetup.PWMMode);

	print_uart0("%d;%d;%d;%d;",
				(signed int)fcSetup.pid_X_GyroACCFactMin,
				(signed int)fcSetup.pid_X_GyroACCFactMax,
				(signed int)fcSetup.pid_X_IntegralMin,
				(signed int)fcSetup.pid_X_IntegralMax);

	print_uart0("%d;%d;%d;%d;",
				(signed int)fcSetup.pid_Y_GyroACCFactMin,
				(signed int)fcSetup.pid_Y_GyroACCFactMax,
				(signed int)fcSetup.pid_Y_IntegralMin,
				(signed int)fcSetup.pid_Y_IntegralMax);

	print_uart0("%d;%d;%d;%d;",
				(int)fcSetup.pid_X_AccX_Fact*10,
				(int)fcSetup.pid_X_P*10,
				(int)fcSetup.pid_X_D*10,
				(signed int)(fcSetup.pid_X_PitchFact*1000));

	print_uart0("%d;%d;%d;%d;",
				(int)fcSetup.pid_Y_AccY_Fact*10,
				(int)fcSetup.pid_Y_P*10,
				(int)fcSetup.pid_Y_D*10,
				(signed int)(fcSetup.pid_Y_PitchFact*1000));

	print_uart0("%d;%d;%d;%d;",
				(signed int)(fcSetup.pid_GyroPitchFact*1000),
				(signed int)(fcSetup.pid_StickFact*1000),
				(int)(fcSetup.pid_PitchP),
				(int)(fcSetup.pid_PitchD));

	print_uart0("%d;%d;%d;%d;%d;%d;",
				(int)(fcSetup.pid_X_GyroFact*1000),
				(int)(fcSetup.pid_X_I*10000),
				(int)(fcSetup.pid_X_GyroACCFact*1000),
				(int)(fcSetup.pid_Y_GyroFact*1000),
				(int)(fcSetup.pid_Y_I*10000),
				(int)(fcSetup.pid_Y_GyroACCFact*1000));


	print_uart0("%d;%d;%d;%d;%d;",
				(int)(fcSetup.pd_X_P*100),
				(int)(fcSetup.pd_X_D*100),
				(int)(fcSetup.pd_Y_P*100),
				(int)(fcSetup.pd_Y_D*100),
				(int)(fcSetup.pd_GyroPitchFact*100));

	print_uart0("%d;%d;%d;%d;",
				(int)(fcSetup.pd_GyroStickFact*100),
				(int)(fcSetup.pd_stickFact*100),
				(int)(fcSetup.pd_X_sensorFact*100),
				(int)(fcSetup.pd_X_PitchFact*100));


	print_uart0("%d;%d;%d;%d;",
				(int)(fcSetup.pd_Y_sensorFact*100),
				(int)(fcSetup.pd_Y_PitchFact*100),
				(int)(fcSetup.pd_PitchP),
				(int)(fcSetup.pd_PitchD));

	print_uart0("%d;%d;%d;%d;",
				(int)fcSetup.escType,
				(int)fcSetup.escMax,
				(int)fcSetup.escBaseAdr,
				(int)fcSetup.escAdrHop);

	print_uart0("%d;%d;%d;%d;%d;",
				(int)sysSetup.CYCLE.calcCycle,
				(int)sysSetup.CYCLE.telemetrieCycle,
				(int)sysSetup.CYCLE.componentCycle,
				(int)sysSetup.CYCLE.AdcClockDiv,
				(int)sysSetup.PWM.MaxValue);

	print_uart0("%d;%d;%d;%d;%d;",
				(int)sysSetup.PWM.MinValue,
				(int)sysSetup.PWM.MaxMultichannel,
				(int)sysSetup.PWM.mcOffset,
				(int)sysSetup.PWM.mcOffset2,
				(int)sysSetup.PWM.scOffsetBase);

	print_uart0("%d;%d;%d;%d;%d;",
				(int)sysSetup.PWM.mpOffset[0],
				(int)sysSetup.PWM.mpOffset[1],
				(int)sysSetup.PWM.mpOffset[2],
				(int)sysSetup.PWM.mpOffset[3],
				(int)sysSetup.PWM.mpOffset[4]);

	print_uart0("%d;%d;%d;%d;",
				(int)sysSetup.PWM.mpOffset[5],
				(int)sysSetup.PWM.mpOffset[6],
				(int)sysSetup.PWM.mpOffset[7],
				(int)sysSetup.PWM.mpOffset[8]);

	print_uart0("%d;%d;%d;%d;%d;",
				(int)fcSetup.userSetting[0],
				(int)fcSetup.userSetting[1],
				(int)fcSetup.userSetting[2],
				(int)fcSetup.userSetting[3],
				(int)fcSetup.userSetting[4]);

	print_uart0("%d;%d;%d;%d;%d;",
				(int)fcSetup.userSetting[5],
				(int)fcSetup.userSetting[6],
				(int)fcSetup.userSetting[7],
				(int)fcSetup.userSetting[8],
				(int)fcSetup.userSetting[9]);

	print_uart0("%d;%d;%d;%d;%d;",
				(int)fcSetup.userSetting[10],
				(int)fcSetup.userSetting[11],
				(int)fcSetup.userSetting[12],
				(int)fcSetup.userSetting[13],
				(int)fcSetup.userSetting[14]);

	print_uart0("%d;%d;%d;%d;%d",
				(int)fcSetup.userSetting[15],
				(int)fcSetup.userSetting[16],
				(int)fcSetup.userSetting[17],
				(int)fcSetup.userSetting[18],
				(int)fcSetup.userSetting[19]);

	print_uart0(";00#");

}





void printDebug5(void)
{

	print_uart0("FCd6;");
	print_uart0("%X;%X;%X;%X;%X;",
				(int)fcSetup.chan[0],
				(int)fcSetup.chan[1],
				(int)fcSetup.chan[2],
				(int)fcSetup.chan[3],
				(int)fcSetup.chan[4]);

	print_uart0("%X;%X;%X;%X;%X;",
				(int)fcSetup.chan[5],
				(int)fcSetup.chan[6],
				(int)fcSetup.chan[7],
				(int)fcSetup.chan[8],
				(int)fcSetup.chan[9]);

	print_uart0("%X;%X;%X;%X;",
				(int)fcSetup.chan[10],
				(int)fcSetup.chan[11],
				(int)fcSetup.components[0],
				(int)fcSetup.components[1]);

	print_uart0("%X;%X;%X;",
				(int)fcSetup.components[2],
				(int)fcSetup.nickServoOn,
				(int)fcSetup.rollServoOn);

	print_uart0("%X;%X;%X;%X;",
				(int)fcSetup.pid_PitchStickFact,
				(int)fcSetup.pid_throttleOffset,
				(int)fcSetup.pd_PitchStickFact,
				(int)fcSetup.pd_throttleOffset);

	print_uart0("%X;%X;",
				(int)fcSetup.sysGasMin,
				(int)fcSetup.sysRcGasMax);

	print_uart0("%X;%X;%X;%X;",
				(int)fcSetup.sysEmergencyGas,
				(int)fcSetup.sysEmergencyGasDuration,
				(int)fcSetup.sysLowVoltage,
				(int)fcSetup.sysMainDirection);

	print_uart0("%X;%X;%X;",
				(int)fcSetup.serialControl,
				(int)fcSetup.sysGasMax,
				(int)fcSetup.PWMMode);

	print_uart0("%X;%X;%X;%X;",
				(signed int)fcSetup.pid_X_GyroACCFactMin,
				(signed int)fcSetup.pid_X_GyroACCFactMax,
				(signed int)fcSetup.pid_X_IntegralMin,
				(signed int)fcSetup.pid_X_IntegralMax);

	print_uart0("%X;%X;%X;%X;",
				(signed int)fcSetup.pid_Y_GyroACCFactMin,
				(signed int)fcSetup.pid_Y_GyroACCFactMax,
				(signed int)fcSetup.pid_Y_IntegralMin,
				(signed int)fcSetup.pid_Y_IntegralMax);

	print_uart0("%X;%X;%X;%X;",
				//fcSetup.reserved0,
				(int)fcSetup.pid_X_AccX_Fact*10,
				(int)fcSetup.pid_X_P*10,
				(int)fcSetup.pid_X_D*10,
				(signed int)(fcSetup.pid_X_PitchFact*1000));

	print_uart0("%X;%X;%X;%X;",
				(int)fcSetup.pid_Y_AccY_Fact*10,
				(int)fcSetup.pid_Y_P*10,
				(int)fcSetup.pid_Y_D*10,
				(signed int)(fcSetup.pid_Y_PitchFact*1000));

	print_uart0("%X;%X;%X;%X;",
				(signed int)(fcSetup.pid_GyroPitchFact*1000),
				(signed int)(fcSetup.pid_StickFact*1000),
				(int)(fcSetup.pid_PitchP),
				(int)(fcSetup.pid_PitchD));

	print_uart0("%X;%X;%X;%X;%X;%X;",
				(int)(fcSetup.pid_X_GyroFact*1000),
				(int)(fcSetup.pid_X_I*10000),
				(int)(fcSetup.pid_X_GyroACCFact*1000),
				(int)(fcSetup.pid_Y_GyroFact*1000),
				(int)(fcSetup.pid_Y_I*10000),
				(int)(fcSetup.pid_Y_GyroACCFact*1000));


	print_uart0("%X;%X;%X;%X;%X;",
				(int)(fcSetup.pd_X_P*100),
				(int)(fcSetup.pd_X_D*100),
				(int)(fcSetup.pd_Y_P*100),
				(int)(fcSetup.pd_Y_D*100),
				(int)(fcSetup.pd_GyroPitchFact*100));

	print_uart0("%X;%X;%X;%X;",
				(int)(fcSetup.pd_GyroStickFact*100),
				(int)(fcSetup.pd_stickFact*100),
				(int)(fcSetup.pd_X_sensorFact*100),
				(int)(fcSetup.pd_X_PitchFact*100));


	print_uart0("%X;%X;%X;%X;",
				(int)(fcSetup.pd_Y_sensorFact*100),
				(int)(fcSetup.pd_Y_PitchFact*100),
				(int)(fcSetup.pd_PitchP),
				(int)(fcSetup.pd_PitchD));

	print_uart0("%X;%X;%X;%X;",
				(int)fcSetup.escType,
				(int)fcSetup.escMax,
				(int)fcSetup.escBaseAdr,
				(int)fcSetup.escAdrHop);

	print_uart0("%X;%X;%X;%X;%X;",
				(int)sysSetup.CYCLE.calcCycle,
				(int)sysSetup.CYCLE.telemetrieCycle,
				(int)sysSetup.CYCLE.componentCycle,
				(int)sysSetup.CYCLE.AdcClockDiv,
				(int)sysSetup.PWM.MaxValue);

	print_uart0("%X;%X;%X;%X;%X;",
				(int)sysSetup.PWM.MinValue,
				(int)sysSetup.PWM.MaxMultichannel,
				(int)sysSetup.PWM.mcOffset,
				(int)sysSetup.PWM.mcOffset2,
				(int)sysSetup.PWM.scOffsetBase);


	print_uart0("%X;%X;%X;%X;%X;",
				(int)sysSetup.PWM.mpOffset[0],
				(int)sysSetup.PWM.mpOffset[1],
				(int)sysSetup.PWM.mpOffset[2],
				(int)sysSetup.PWM.mpOffset[3],
				(int)sysSetup.PWM.mpOffset[4]);

	print_uart0("%X;%X;%X;%X;",
				(int)sysSetup.PWM.mpOffset[5],
				(int)sysSetup.PWM.mpOffset[6],
				(int)sysSetup.PWM.mpOffset[7],
				(int)sysSetup.PWM.mpOffset[8]);

	print_uart0("%X;%X;%X;%X;%X;",
				(int)fcSetup.userSetting[0],
				(int)fcSetup.userSetting[1],
				(int)fcSetup.userSetting[2],
				(int)fcSetup.userSetting[3],
				(int)fcSetup.userSetting[4]);

	print_uart0("%X;%X;%X;%X;%X;",
				(int)fcSetup.userSetting[5],
				(int)fcSetup.userSetting[6],
				(int)fcSetup.userSetting[7],
				(int)fcSetup.userSetting[8],
				(int)fcSetup.userSetting[9]);

	print_uart0("%X;%X;%X;%X;%X;",
				(int)fcSetup.userSetting[10],
				(int)fcSetup.userSetting[11],
				(int)fcSetup.userSetting[12],
				(int)fcSetup.userSetting[13],
				(int)fcSetup.userSetting[14]);

	print_uart0("%X;%X;%X;%X;%X",
				(int)fcSetup.userSetting[15],
				(int)fcSetup.userSetting[16],
				(int)fcSetup.userSetting[17],
				(int)fcSetup.userSetting[18],
				(int)fcSetup.userSetting[19]);

	print_uart0(";00#");

}

void printDebug6(void) {
	print_uart0("FCd6;");

	print_uart0("%d;%d;%d;%d;%d;%d;%d;%d;",
			(int)ADC_standStill[ADC_NICK],
			(int)ADC_standStill[ADC_ROLL],
			(int)ADC_standStill[ADC_PITCH],
			(int)ADC_standStill[ADC_ACCX],
			(int)ADC_standStill[ADC_ACCY],
			(int)ADC_standStill[ADC_ACCZ],
			(int)ADC_standStill[AIRPRESSURE],
			(int)(float)(ADC_standStill[UBAT]/ubatDivider));

	print_uart0("00#");

}

void printDebug7(void)
{
	print_uart0("FCd7;");
	print_uart0(" to be defined by you");
	print_uart0(";00#");

}














