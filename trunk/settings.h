/*
	file    settings.h
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



#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "compass.h"
#include "pwmin.h"

typedef struct
{

	unsigned char	pd_PitchP;						//Gyr P
	unsigned char	pd_PitchD;

	unsigned char	sysGasMin;						//Minimum Gas after Engine startup or with no Gas from RC
	unsigned char	sysGasMax;						//Maximum Gas level reachable all in also
													//including system Gas for stabilisation
	unsigned char	sysRcGasMax;					//Maximum Gas level reachable via RC
	unsigned char	sysEmergencyGas;				//Gas value in emergency situation
	unsigned char	sysEmergencyGasDuration;		//how long to hold Gas in emergency

	unsigned char	nickServoOn;				//nick servo enable with 1
	unsigned char	nickServoChan;				//nick servo enable with 1
	unsigned char	nickServoInvert;			//invert nick signals
	unsigned char	nickServoPos;
	unsigned char	nickServoForce;				//how strict to compensate
	unsigned char	nickServoMin;				//Min value for nick servo
	unsigned char	nickServoMax;				//Max value for nick servo


	unsigned char	rollServoOn;				//roll servo enable with 1
	unsigned char	rollServoChan;				//roll servo enable with 1
	unsigned char	rollServoInvert;			//invert roll signals
	unsigned char	rollServoPos;
	unsigned char	rollServoForce;				//how strict to compensate
	unsigned char	rollServoMin;				//Min value for roll servo
	unsigned char	rollServoMax;				//Max value for roll servo

	unsigned char	sysLowVoltage;				//low Voltage warning
	unsigned char	sysMainDirection;			//x or + setup 0, 45, 90....
	unsigned char	serialControl;				//control via serial port


	unsigned char	PWMMode;					//PWMMode Mode
												//0 = Sum signal
												//1 = 9 channel PWMMode input

	unsigned char	BTMode;						// mode of BlueTooth operation
												// 0 = uart 0 only BT off
												// 1 = uart0 + uart3 while uart3 = uart0
												// 2 = no uart0 uart3 only

	unsigned char	ADCModeNick;				// the mode of ADC 0 for mems or 1 for piezo
	unsigned char	ADCModeRoll;				// this triggers mainly the piezo auto setup
	unsigned char	ADCModePitch;				// thats not needed for mems

	unsigned char	ADCDriftNick;				// The Piezos tend to drift. So here we can change
	unsigned char	ADCDriftRoll;				// the value added each second which schould help to
	unsigned char	ADCDriftPitch;				// Minimize the effect
												// hint: the sensors drift less once they warm up. due the fact we loose anything during change of battery
												// it may be useful to have two separate setting where
												// just the drift differs.
												// form mems keep this 0 at any time

	unsigned char 	calcMode;					//define which mode for calculation to be used
												//0 P mode
												//1 P_I mode
												//2 P_I_D mode (no working code yet)
												//3 Kalman mode (no working setup yet)
												//4 experimental mode (no working setup yet) use with caution there you
												// have to add your own code first

	unsigned char	pd_throttleOffset;
	unsigned char	pid_throttleOffset;

	unsigned char	escType;
	unsigned char	escMax;
	unsigned char	escBaseAdr;
	unsigned char	escAdrHop;

	unsigned char 	HMC5843_Mode;				// mode of operation
	unsigned char 	HMC5843_Bias;				// normal positive or negative bias mode
	unsigned char 	HMC5843_Delay;				// ADC delay
	unsigned char 	HMC5843_Gain;				// gain voltage
	unsigned char 	HMC5843_Rate;				// rate of operation

	signed int 	pid_Pitch_IntegralMax;
	signed int 	pid_Pitch_IntegralMin;
	signed int 	pid_X_GyroACCFactMin;
	signed int 	pid_X_GyroACCFactMax;
	signed int 	pid_X_IntegralMin;
	signed int 	pid_X_IntegralMax;
	signed int 	pid_Y_GyroACCFactMin;
	signed int 	pid_Y_GyroACCFactMax;
	signed int 	pid_Y_IntegralMin;
	signed int 	pid_Y_IntegralMax;
	signed int	X_sensorBias;
	signed int	Y_sensorBias;
	signed int	PitchSensorBias;

	//float	pid_Pitch_GyrFact;
	float	pid_PitchThrottleFact;
	float	pid_PitchP;						//pi Pitch P
	float	pid_PitchI;
	float	pid_PitchD;
	float 	pid_X_P;
	float	pid_X_I;
	float 	pid_X_D;
	float 	pid_Y_P;
	float	pid_Y_I;
	float 	pid_Y_D;

	float 	pid_X_AccX_Fact;
	float 	pid_Y_AccY_Fact;

	float 	pd_X_AccX_Fact;
	float 	pd_Y_AccY_Fact;




	float	pid_GyroPitchFact;
	float	pid_StickFact;
	float	pid_PitchStickFact;


	float	pid_X_GyroFact;
	float	pid_X_PitchFact;
	float	pid_X_GyroACCFact;

	float	pid_Y_GyroFact;
	float	pid_Y_PitchFact;
	float	pid_Y_GyroACCFact;

	float 	pd_X_P;
	float 	pd_X_D;
	float 	pd_Y_P;
	float 	pd_Y_D;

	float	pd_GyroPitchFact;
	float	pd_GyroStickFact;
	float	pd_PitchStickFact;
	float 	pd_stickFact;
	float	pd_X_sensorFact;
	float	pd_X_PitchFact;
	float	pd_Y_sensorFact;
	float	pd_Y_PitchFact;

	unsigned char	chan[12];					//RC setup
	unsigned char	components[3];				//component setup barometer, compass, gps
	unsigned char userSetting[20];				//20 User set Values not yet available

}FLIGHTSETTINGS;

typedef struct
{
	int calcCycle;
	int telemetrieCycle;
	int componentCycle;
	char AdcClockDiv;
}CYCLECONTROL;

typedef struct
{
	int MaxValue;
	int MinValue;
	char MaxMultichannel;
	int mcOffset;
	int mcOffset2;
	int scOffsetBase;
	int mpOffset[9];
} PWMSETUP;

typedef struct
{
	int X_off;
	int Y_off;
	int Z_off;
	int X_range;
	int Y_range;
	int Z_range;
} MM3SETUP;

typedef struct
{
	CYCLECONTROL CYCLE;
	PWMSETUP PWM;
	MM3SETUP MM3;
}SYSTEMSETTINGS;

typedef struct {
	unsigned char 	settingNum;
	unsigned char 	pid_X_GyroACCFactMin;
	unsigned char 	pid_X_GyroACCFactMax;
	unsigned char 	pid_X_IntegralMin;
	unsigned char 	pid_X_IntegralMax;
	unsigned char	pid_X_AccX_Fact;
	unsigned char	pid_X_GyroSumFact;
	unsigned char	pid_X_P;
	unsigned char	pid_X_I;
	unsigned char	pid_X_D;
	unsigned char   pid_X_PitchSumFact;
	unsigned char	pid_X_SumFact;
	unsigned char	X_sensorBias;
	unsigned char	X_sensorBiasNeg;
	unsigned char 	pid_Y_GyroACCFactMin;
	unsigned char 	pid_Y_GyroACCFactMax;
	unsigned char 	pid_Y_IntegralMin;
	unsigned char	pid_Y_IntegralMax;
	unsigned char	pid_Y_AccY_Fact;
	unsigned char	pid_Y_GyroSumFact;
	unsigned char	pid_Y_P;
	unsigned char	pid_Y_I;
	unsigned char	pid_Y_D;
	unsigned char   pid_Y_PitchSumFact;
	unsigned char	pid_Y_SumFact;
	unsigned char	Y_sensorBias;
	unsigned char	Y_sensorBiasNeg;
	unsigned char	PitchSensorBias;
	unsigned char	PitchSensorBiasNeg;
	unsigned char	pid_GyroPitchFact;
	unsigned char	pid_StickFact;
	unsigned char	pid_PitchStickFact;
	unsigned char	pid_throttleOffset;
	unsigned char 	pid_PitchThrottleFact;
	unsigned char	pid_PitchP;
	unsigned char	pid_PitchI;
	unsigned char	pid_PitchD;
	unsigned char 	pid_Pitch_IntegralMax;
	unsigned char 	pid_Pitch_IntegralMin;
	unsigned char	pd_throttleOffset;
	unsigned char	pd_X_P_Fact;
	unsigned char	pd_X_D_Fact;
	unsigned char	pd_Y_P_Fact;
	unsigned char	pd_Y_D_Fact;
	unsigned char	pd_X_AccX_Fact;
	unsigned char	pd_Y_AccY_Fact;
	unsigned char	pd_X_GyroSumFact;
	unsigned char   pd_X_PitchSumFact;
	unsigned char	pd_Y_GyroSumFact;
	unsigned char   pd_Y_PitchSumFact;
	unsigned char	pd_PitchP;
	unsigned char	pd_PitchD;
	unsigned char	pd_GyroPitchFact;
	unsigned char	pd_StickFact;
	unsigned char	pd_PitchStickFact;
	unsigned char	barOn;
	unsigned char	compOn;
	unsigned char	gpsOn;
	unsigned char	nickServoOn;
	unsigned char	nickServoChan;
	unsigned char	nickServoInvert;
	unsigned char	nickServoPos;
	unsigned char	nickServoForce;
	unsigned char	nickServoMin;
	unsigned char	nickServoMax;

	unsigned char	rollServoOn;
	unsigned char	rollServoChan;
	unsigned char	rollServoInvert;
	unsigned char	rollServoPos;
	unsigned char	rollServoForce;
	unsigned char	rollServoMin;
	unsigned char	rollServoMax;

	unsigned char	sysGasMin;
	unsigned char	sysGasMax;
	unsigned char	sysRcGasMax;
	unsigned char	sysLowVoltage;
	unsigned char	sysEmergencyGas;
	unsigned char	sysEmergencyGasDuration;
	unsigned char	calcMode;
	unsigned char	sysMainDirection;
	unsigned char 	escType;
	unsigned char 	escMax;
	unsigned char 	escBaseAdr;
	unsigned char 	escAdrHop;
	unsigned char 	calcCycle;
	unsigned char 	telemetrieCycle;
	unsigned char 	componentCycle;
	unsigned char 	AdcClockDiv;
	unsigned char	PWMMode;
	unsigned char	BTMode;
	unsigned char	ADCModeNick;
	unsigned char	ADCModeRoll;
	unsigned char	ADCModePitch;
	unsigned char	ADCDriftNick;
	unsigned char	ADCDriftRoll;
	unsigned char	ADCDriftPitch;
	unsigned char 	MaxValue;
	unsigned char 	MinValue;
	unsigned char 	MaxMultichannel;
	unsigned char 	mcOffset;
	unsigned char 	mcOffset2;
	unsigned char 	scOffsetBase;
	unsigned char 	HMC5843_Mode;
	unsigned char 	HMC5843_Bias;
	unsigned char 	HMC5843_Delay;
	unsigned char 	HMC5843_Gain;
	unsigned char 	HMC5843_Rate;
	unsigned char	chan[12];
	unsigned char 	mpOffset[9];
	unsigned char	userSetting[21];
} INCOMINGSETTINGS;

#define airpressureComponent 0
#define compassComponent 1
#define gpsComponent 2

typedef struct
{
	INCOMINGSETTINGS settingsStore;
	unsigned char	trash[102];
} SETTINGSSTORE;


unsigned int adcRate;


volatile INCOMINGSETTINGS setupCache;

volatile INCOMINGSETTINGS setupCacheT0;
volatile INCOMINGSETTINGS setupCacheT1;
volatile INCOMINGSETTINGS setupCacheT2;
volatile INCOMINGSETTINGS setupCacheT3;
volatile INCOMINGSETTINGS setupCacheT4;
volatile INCOMINGSETTINGS setupCacheT5;

volatile FLIGHTSETTINGS fcSetup;
volatile SYSTEMSETTINGS sysSetup;

volatile SETTINGSSTORE setupStore;



void initSettings(void);
void setToInSettings(void);
void retriveSetting(void);
void setTempToInSetting(char setNum);
void readSetup(char readSet);
void uncacheSetting(char setNum);
void retriveInitialSettings(void);
void setFcSetupDefault(void);



void SetSysSetupDefault(void);
void writeSetup(void);

#endif
