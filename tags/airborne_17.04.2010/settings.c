/*
	file    settings.c
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


#include "settings.h"
#include "iapcore.h"
#include "command.h"
#include "pwmin.h"
//#include "analog.h"

char slot = 0; //for now this is hard coded slot can be 0 to 3



void readSetup(char readSet)
{
	ReadEEPROM(slot, readSet);
	setupCache = (INCOMINGSETTINGS)setupStore.settingsStore;
}


void uncacheSetting(char setNum)
{
	switch (setNum)
		{
			case 0:
				setupCache = (INCOMINGSETTINGS)setupCacheT0;
			break;
			case 1:
				setupCache = (INCOMINGSETTINGS)setupCacheT1;
			break;
			case 2:
				setupCache = (INCOMINGSETTINGS)setupCacheT2;
			break;
			case 3:
				setupCache = (INCOMINGSETTINGS)setupCacheT3;
			break;
			case 4:
				setupCache = (INCOMINGSETTINGS)setupCacheT4;
			break;
			case 5:
				setupCache = (INCOMINGSETTINGS)setupCacheT5;
			break;
		}

}

void retriveInitialSettings(void)
{

	readSetup(1);
	setupCacheT1 = (INCOMINGSETTINGS)setupStore.settingsStore;
	readSetup(2);
	setupCacheT2 = (INCOMINGSETTINGS)setupStore.settingsStore;
	readSetup(3);
	setupCacheT3 = (INCOMINGSETTINGS)setupStore.settingsStore;
	readSetup(4);
	setupCacheT4 = (INCOMINGSETTINGS)setupStore.settingsStore;
	readSetup(5);
	setupCacheT5 = (INCOMINGSETTINGS)setupStore.settingsStore;
	readSetup(0);
	setupCacheT0 = (INCOMINGSETTINGS)setupStore.settingsStore;
	setupCache = (INCOMINGSETTINGS)setupStore.settingsStore;
	setToInSettings();
}
//todo fix initial setup data

void setFcSetupDefault(void)
{
	setupCache.settingNum 							= 0;
	setupCache.barOn 								= 0;
	setupCache.compOn 								= 0;
	setupCache.gpsOn 								= 0;
//todo
	setupCache.nickServoOn 							= 0;
	setupCache.rollServoOn 							= 0;


	setupCache.pid_throttleOffset 					= 0;
	setupCache.pid_PitchStickFact 					= 0;
	setupCache.pd_PitchStickFact 					= 0;
	setupCache.pd_throttleOffset 					= 0;
	setupCache.sysGasMin 							= 5;
	setupCache.sysRcGasMax 							= 120;
	setupCache.sysEmergencyGas 						= 40;
	setupCache.sysEmergencyGasDuration 				= 40;
	setupCache.sysLowVoltage 						= 95;
	setupCache.sysMainDirection 					= 0;
	setupCache.sysGasMax 							= 120;
	setupCache.PWMMode 								= 1;
	setupCache.BTMode 								= 0;
	setupCache.ADCModeNick 							= 0;
	setupCache.ADCModeRoll 							= 0;
	setupCache.ADCModePitch 						= 0;
	setupCache.ADCDriftNick 						= 0;
	setupCache.ADCDriftRoll 						= 0;
	setupCache.ADCDriftPitch 						= 0;
	setupCache.calcMode 							= 0;
	setupCache.pid_X_AccX_Fact 						= 35;
	setupCache.pid_X_GyroSumFact 					= 100;
	setupCache.pid_X_P 								= 110;
	setupCache.pid_X_D 								= 20;
	setupCache.pid_X_I 								= 60;
	setupCache.pid_X_PitchSumFact 					= 50;
	setupCache.pid_X_SumFact 						= 30;
	setupCache.X_sensorBias 						= 0;
	setupCache.X_sensorBiasNeg 						= 0;
	setupCache.pid_Y_AccY_Fact 						= 35;
	setupCache.pid_Y_GyroSumFact 					= 100;
	setupCache.pid_Y_P 								= 110;
	setupCache.pid_Y_D 								= 20;
	setupCache.pid_Y_I 								= 60;
	setupCache.pid_Y_PitchSumFact 					= 50;
	setupCache.pid_Y_SumFact 						= 30;
	setupCache.Y_sensorBias 						= 0;
	setupCache.Y_sensorBiasNeg 						= 0;
	setupCache.pid_GyroPitchFact 					= 1020;
	setupCache.pid_StickFact 						= 50;
	setupCache.pid_PitchThrottleFact 				= 80;
	setupCache.pid_PitchP 							= 60;
	setupCache.pid_PitchI 							= 13;
	setupCache.pid_PitchD 							= 20;
	setupCache.PitchSensorBias 						= 0;
	setupCache.PitchSensorBiasNeg 					= 0;
	setupCache.pd_X_P_Fact 							= 0;
	setupCache.pd_X_D_Fact 							= 0;
	setupCache.pd_Y_P_Fact 							= 0;
	setupCache.pd_Y_D_Fact 							= 0;
	setupCache.pd_PitchP 							= 0;
	setupCache.pd_PitchD 							= 0;
	setupCache.pd_X_AccX_Fact 						= 0;
	setupCache.pd_Y_AccY_Fact 						= 0;
	setupCache.pd_X_GyroSumFact 					= 0;
	setupCache.pd_X_PitchSumFact 					= 0;
	setupCache.pd_Y_GyroSumFact 					= 0;
	setupCache.pd_Y_PitchSumFact 					= 0;
	setupCache.pd_GyroPitchFact 					= 0;
	setupCache.pd_StickFact 						= 0;
	setupCache.pid_X_GyroACCFactMin 				= 8;
	setupCache.pid_X_GyroACCFactMax 				= 8;
	setupCache.pid_X_IntegralMin 					= 20;
	setupCache.pid_X_IntegralMax 					= 20;
	setupCache.pid_Y_GyroACCFactMin 				= 8;
	setupCache.pid_Y_GyroACCFactMax 				= 8;
	setupCache.pid_Y_IntegralMin 					= 20;
	setupCache.pid_Y_IntegralMax 					= 20;
	setupCache.pid_Pitch_IntegralMax 				= 20;
	setupCache.pid_Pitch_IntegralMin 				= 20;
	setupCache.escType 								= 0;
	setupCache.escMax 								= 4;
	setupCache.escBaseAdr 							= 0x52;
	setupCache.escAdrHop 							= 2;
	setupCache.AdcClockDiv							= 120;
	setupCache.calcCycle							= 90;
	setupCache.componentCycle						= 50;
	setupCache.telemetrieCycle						= 10;
	setupCache.MaxMultichannel						= 7;
	setupCache.MaxValue								= 30;
	setupCache.MinValue								= 29;
	setupCache.mcOffset								= 42;
	setupCache.mcOffset2							= 32;
	setupCache.scOffsetBase							= 30;

	setupCache.HMC5843_Mode							= 0;
	setupCache.HMC5843_Bias							= 0;
	setupCache.HMC5843_Delay						= 0;
	setupCache.HMC5843_Gain							= 0;
	setupCache.HMC5843_Rate							= 0;

	setupCache.chan[0] 								= 4;
	setupCache.chan[1] 								= 3;
	setupCache.chan[2] 								= 5;
	setupCache.chan[3] 								= 6;
	setupCache.chan[4] 								= 0;
	setupCache.chan[5] 								= 2;
	setupCache.chan[6] 								= 1;
	setupCache.chan[7] 								= 7;
	setupCache.chan[8] 								= 8;
	setupCache.chan[9] 								= 9;
	setupCache.chan[10] 							= 10;
	setupCache.chan[11] 							= 11;
	setupCache.mpOffset[0]							= 40;
	setupCache.mpOffset[1]							= 40;
	setupCache.mpOffset[2]							= 40;
	setupCache.mpOffset[3]							= 40;
	setupCache.mpOffset[4]							= 40;
	setupCache.mpOffset[5]							= 40;
	setupCache.mpOffset[6]							= 40;
	setupCache.mpOffset[7]							= 40;
	setupCache.mpOffset[8]							= 40;
	setupCache.userSetting[0]						= 0;
	setupCache.userSetting[1]						= 0;
	setupCache.userSetting[2]						= 0;
	setupCache.userSetting[3]						= 0;
	setupCache.userSetting[4]						= 0;
	setupCache.userSetting[5]						= 0;
	setupCache.userSetting[6]						= 0;
	setupCache.userSetting[7]						= 0;
	setupCache.userSetting[8]						= 0;
	setupCache.userSetting[9]						= 0;
	setupCache.userSetting[10]						= 0;
	setupCache.userSetting[11]						= 0;
	setupCache.userSetting[12]						= 0;
	setupCache.userSetting[13]						= 0;
	setupCache.userSetting[14]						= 0;
	setupCache.userSetting[15]						= 0;
	setupCache.userSetting[16]						= 0;
	setupCache.userSetting[17]						= 0;
	setupCache.userSetting[18]						= 0;
	setupCache.userSetting[19]						= 0;
	setupCache.userSetting[20]						= 23;		//23 would mark it as a saveable setting
}

void retriveSetting(void)
{
	uncacheSetting(settingNumRequested);
	setToInSettings();
}

void setTempToInSetting(char setNum)
{
	setupCache.settingNum = setNum;
	print_uart0("FCm0;Settings Requested %d;00#",setNum);
	switch (setNum)
	{
		case 0:
			setupCacheT0 = (INCOMINGSETTINGS)setupCache;
		break;
		case 1:
			setupCacheT1 = (INCOMINGSETTINGS)setupCache;
		break;
		case 2:
			setupCacheT2 = (INCOMINGSETTINGS)setupCache;
		break;
		case 3:
			setupCacheT3 = (INCOMINGSETTINGS)setupCache;
		break;
		case 4:
			setupCacheT4 = (INCOMINGSETTINGS)setupCache;
		break;
		case 5:
			setupCacheT5 = (INCOMINGSETTINGS)setupCache;
		break;
	}

}

void initSettings(void)
{
	retriveInitialSettings();
	if (setupCache.userSetting[20] != 23) {
		setFcSetupDefault();						  //loads a default set of config data
		setToInSettings();
		print_uart0("FCm0;loaded default settings;00#");
	} else {
		setToInSettings();
		print_uart0("FCm0;loaded settings from flash;00#");
	}
}


void writeSetup(void)
{
	WriteEEPROM(slot); //setting can be 0 to 15
}

void setToInSettings(void)
{
	//here we set the runtime data to incoming settings coming from flash ram or via UART
	//we use this place all the time
	//the numbers chosen to be devided by or multiplicated by are found by
	//try and error this may change over time but its not really worth to be a volatile setting

	fcSetup.pd_GyroPitchFact			= (float)setupCache.pd_GyroPitchFact/100;
	fcSetup.pd_GyroStickFact			= (float)setupCache.pd_StickFact/100;
	fcSetup.pd_PitchStickFact			= (float)setupCache.pd_PitchStickFact/100;
	fcSetup.pd_X_sensorFact				= (float)setupCache.pd_X_GyroSumFact/100;
	fcSetup.pd_Y_sensorFact				= (float)setupCache.pd_Y_GyroSumFact/100;
	fcSetup.pd_X_PitchFact  			= (float)setupCache.pd_X_PitchSumFact/100;
	fcSetup.pd_Y_PitchFact  			= (float)setupCache.pd_Y_PitchSumFact/100;
	fcSetup.pd_X_AccX_Fact				= (float)setupCache.pd_X_AccX_Fact/10;
	fcSetup.pd_Y_AccY_Fact				= (float)setupCache.pd_Y_AccY_Fact/10;
	fcSetup.pd_X_P						= (float)setupCache.pd_X_P_Fact/100;
	fcSetup.pd_X_D						= (float)setupCache.pd_X_D_Fact/100;
	fcSetup.pd_Y_P						= (float)setupCache.pd_Y_P_Fact/100;
	fcSetup.pd_Y_D						= (float)setupCache.pd_Y_D_Fact/100;
	fcSetup.pd_PitchP 					= (float)setupCache.pd_PitchP/100;
	fcSetup.pd_PitchD					= (float)setupCache.pd_PitchD/100;
	fcSetup.pd_throttleOffset			= setupCache.pd_throttleOffset;
	fcSetup.pd_PitchStickFact			= setupCache.pd_PitchStickFact;


	fcSetup.pid_StickFact				= (float)setupCache.pid_StickFact/10;
	fcSetup.pid_throttleOffset			= setupCache.pid_throttleOffset;

	fcSetup.pid_PitchP					= (float)setupCache.pid_PitchP/100;
	fcSetup.pid_PitchI					= (float)setupCache.pid_PitchI/100;
	fcSetup.pid_PitchD					= (float)setupCache.pid_PitchD/100;
	fcSetup.pid_GyroPitchFact			= (float)setupCache.pid_GyroPitchFact/100;
	fcSetup.pid_PitchThrottleFact		= (float)setupCache.pid_PitchThrottleFact/100;
	fcSetup.pid_PitchStickFact			= (float)setupCache.pid_PitchStickFact/10;
	fcSetup.PitchSensorBias				= (float)(setupCache.PitchSensorBias + (setupCache.PitchSensorBiasNeg*-1));

	fcSetup.pid_Pitch_IntegralMax		= (signed int)setupCache.pid_Pitch_IntegralMax * 10;
	fcSetup.pid_Pitch_IntegralMin		= (signed int)setupCache.pid_Pitch_IntegralMin * 10 *-1;

	fcSetup.pid_X_P  					= (float)setupCache.pid_X_P/100;
	fcSetup.pid_X_I  					= (float)setupCache.pid_X_I/30000;
	fcSetup.pid_X_D	  					= (float)setupCache.pid_X_D/100;
	fcSetup.pid_X_AccX_Fact				= (float)setupCache.pid_X_AccX_Fact/100;
	fcSetup.pid_X_PitchFact  			= (float)setupCache.pid_X_PitchSumFact/100;
	fcSetup.pid_X_GyroACCFact	  		= (float)setupCache.pid_X_SumFact/100;
	fcSetup.pid_X_GyroFact				= (float)setupCache.pid_X_GyroSumFact/100;
	fcSetup.X_sensorBias				= (float)(setupCache.X_sensorBias + (setupCache.X_sensorBiasNeg*-1));

	fcSetup.pid_X_GyroACCFactMin		= (signed int)setupCache.pid_X_GyroACCFactMin*10*-1;
	fcSetup.pid_X_GyroACCFactMax		= (signed int)setupCache.pid_X_GyroACCFactMax*10;
	fcSetup.pid_X_IntegralMin			= (signed int)setupCache.pid_X_IntegralMin*100*-1;
	fcSetup.pid_X_IntegralMax			= (signed int)setupCache.pid_X_IntegralMax*100;


	fcSetup.pid_Y_P  					= (float)setupCache.pid_Y_P/100;
	fcSetup.pid_Y_I  					= (float)setupCache.pid_Y_I/30000;
	fcSetup.pid_Y_D	  					= (float)setupCache.pid_Y_D/100;
	fcSetup.pid_Y_AccY_Fact				= (float)setupCache.pid_Y_AccY_Fact/100;
	fcSetup.pid_Y_PitchFact  			= (float)setupCache.pid_Y_PitchSumFact/100;
	fcSetup.pid_Y_GyroACCFact  			= (float)setupCache.pid_Y_SumFact/100;
	fcSetup.pid_Y_GyroFact				= (float)setupCache.pid_Y_GyroSumFact/100;
	fcSetup.Y_sensorBias				= (float)(setupCache.Y_sensorBias + (setupCache.Y_sensorBiasNeg*-1));

	fcSetup.pid_Y_GyroACCFactMin		= (signed int)setupCache.pid_Y_GyroACCFactMin*10*-1;
	fcSetup.pid_Y_GyroACCFactMax		= (signed int)setupCache.pid_Y_GyroACCFactMax*10;
	fcSetup.pid_Y_IntegralMin			= (signed int)setupCache.pid_Y_IntegralMin*100*-1;
	fcSetup.pid_Y_IntegralMax			= (signed int)setupCache.pid_Y_IntegralMax*100;

	fcSetup.components[airpressureComponent] 	= setupCache.barOn;
	fcSetup.components[compassComponent]	 	= setupCache.compOn;
	fcSetup.components[gpsComponent]		 	= setupCache.gpsOn;
	fcSetup.PWMMode						= setupCache.PWMMode;
	fcSetup.BTMode						= setupCache.BTMode;
	fcSetup.ADCModeNick					= setupCache.ADCModeNick;
	fcSetup.ADCModeRoll					= setupCache.ADCModeRoll;
	fcSetup.ADCModePitch				= setupCache.ADCModePitch;
	fcSetup.ADCDriftNick				= setupCache.ADCDriftNick;
	fcSetup.ADCDriftRoll				= setupCache.ADCDriftRoll;
	fcSetup.ADCDriftPitch				= setupCache.ADCDriftPitch;
	fcSetup.calcMode					= setupCache.calcMode;
	fcSetup.sysGasMin					= setupCache.sysGasMin;
	fcSetup.sysGasMax			 	 	= setupCache.sysGasMax;
	fcSetup.sysRcGasMax					= setupCache.sysRcGasMax;
	fcSetup.sysEmergencyGas				= setupCache.sysEmergencyGas;
	fcSetup.sysEmergencyGasDuration		= setupCache.sysEmergencyGasDuration;

	fcSetup.nickServoOn					= setupCache.nickServoOn;
	fcSetup.nickServoChan				= setupCache.nickServoChan;
	fcSetup.nickServoForce				= setupCache.nickServoForce;
	fcSetup.nickServoMin				= setupCache.nickServoMin;
	fcSetup.nickServoMax				= setupCache.nickServoMax;
	fcSetup.nickServoPos				= setupCache.nickServoPos;
	fcSetup.nickServoInvert				= setupCache.nickServoInvert;
	fcSetup.rollServoOn					= setupCache.rollServoOn;
	fcSetup.rollServoChan				= setupCache.rollServoChan;
	fcSetup.rollServoForce				= setupCache.rollServoForce;
	fcSetup.rollServoMin				= setupCache.rollServoMin;
	fcSetup.rollServoMax				= setupCache.rollServoMax;
	fcSetup.rollServoPos				= setupCache.rollServoPos;
	fcSetup.rollServoInvert				= setupCache.rollServoInvert;


	fcSetup.sysLowVoltage				= setupCache.sysLowVoltage;
	fcSetup.sysMainDirection			= setupCache.sysMainDirection;
	fcSetup.escType						= setupCache.escType;
	fcSetup.escMax						= setupCache.escMax;
	fcSetup.escBaseAdr					= setupCache.escBaseAdr;
	fcSetup.escAdrHop					= setupCache.escAdrHop;
	fcSetup.HMC5843_Mode				= setupCache.HMC5843_Mode;
	fcSetup.HMC5843_Bias				= setupCache.HMC5843_Bias;
	fcSetup.HMC5843_Delay				= setupCache.HMC5843_Delay;
	fcSetup.HMC5843_Gain				= setupCache.HMC5843_Gain;
	fcSetup.HMC5843_Rate				= setupCache.HMC5843_Rate;
	fcSetup.userSetting[0]				= setupCache.userSetting[0];
	fcSetup.userSetting[1]				= setupCache.userSetting[1];
	fcSetup.userSetting[2]				= setupCache.userSetting[2];
	fcSetup.userSetting[3]				= setupCache.userSetting[3];
	fcSetup.userSetting[4]				= setupCache.userSetting[4];
	fcSetup.userSetting[5]				= setupCache.userSetting[5];
	fcSetup.userSetting[6]				= setupCache.userSetting[6];
	fcSetup.userSetting[7]				= setupCache.userSetting[7];
	fcSetup.userSetting[8]				= setupCache.userSetting[8];
	fcSetup.userSetting[9]				= setupCache.userSetting[9];
	fcSetup.userSetting[10]				= setupCache.userSetting[10];
	fcSetup.userSetting[11]				= setupCache.userSetting[11];
	fcSetup.userSetting[12]				= setupCache.userSetting[12];
	fcSetup.userSetting[13]				= setupCache.userSetting[13];
	fcSetup.userSetting[14]				= setupCache.userSetting[14];
	fcSetup.userSetting[15]				= setupCache.userSetting[15];
	fcSetup.userSetting[16]				= setupCache.userSetting[16];
	fcSetup.userSetting[17]				= setupCache.userSetting[17];
	fcSetup.userSetting[18]				= setupCache.userSetting[18];
	fcSetup.userSetting[19]				= setupCache.userSetting[19];

	sysSetup.CYCLE.AdcClockDiv			= setupCache.AdcClockDiv;
	sysSetup.CYCLE.calcCycle			= setupCache.calcCycle * 10;
	adcRate 							= (unsigned int)sysSetup.CYCLE.calcCycle / 8;
	sysSetup.CYCLE.componentCycle		= setupCache.componentCycle;
	sysSetup.CYCLE.telemetrieCycle		= setupCache.telemetrieCycle * 100;
	sysSetup.PWM.MaxMultichannel		= setupCache.MaxMultichannel;
	sysSetup.PWM.MaxValue				= setupCache.MaxValue * 100;
	sysSetup.PWM.MinValue				= setupCache.MinValue * 10;
	sysSetup.PWM.mcOffset				= setupCache.mcOffset * 10;
	sysSetup.PWM.mcOffset2				= setupCache.mcOffset2;
	sysSetup.PWM.scOffsetBase			= setupCache.scOffsetBase * 10;

	sysSetup.PWM.mpOffset[0]			= setupCache.mpOffset[0] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[1]			= setupCache.mpOffset[1] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[2]			= setupCache.mpOffset[2] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[3]			= setupCache.mpOffset[3] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[4]			= setupCache.mpOffset[4] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[5]			= setupCache.mpOffset[5] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[6]			= setupCache.mpOffset[6] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[7]			= setupCache.mpOffset[7] + sysSetup.PWM.scOffsetBase;
	sysSetup.PWM.mpOffset[8]			= setupCache.mpOffset[8] + sysSetup.PWM.scOffsetBase;

	PWM_THROTTLE     					= setupCache.chan[0];
	PWM_R   		 					= setupCache.chan[1];
	PWM_N    		 					= setupCache.chan[2];
	PWM_G   		 					= setupCache.chan[3];
	PWM_POTI1  		 					= setupCache.chan[4];
	PWM_POTI2  		 					= setupCache.chan[5];
	PWM_POTI3  		 					= setupCache.chan[6];
	PWM_user4  		 					= setupCache.chan[7];
	PWM_user5  		 					= setupCache.chan[8];
	PWM_user6  		 					= setupCache.chan[9];
	PWM_user7  							= setupCache.chan[10];
	PWM_user8  		 					= setupCache.chan[11];




}
