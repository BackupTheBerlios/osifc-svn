/*
	file    serialUI.c
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
#include "arch/settings.h"
#include "arch/led.h"
#include "arch/analog.h"
#include "io/engines.h"
#include "io/i2c.h"
#include "io/output.h"
#include "fc/osifc.h"
#include "interface/serialui.h"
#include "interface/command.h"

//commandType will be the Type of Command received.
//0 = requestDebugMessage
//1 = setup Data
//2 = update ADCRate will get a system setting later so may get removed later
//3 = switch setting setupCache.settingNum will get used behind the scenes


void serialUI(void) {


	if (commandRetrived == 1) {

		if (commandType == 0) {
			checkCmd();
			commandRetrived = 0;
		}

		if (commandType == 1) {
			if (setCmd == 0) {								//this is to send in data. the data is not yet flashed
				led_switch(2);								//data can be send in at any time also at flight time
															//it will be used with the next cycle
				print_uart0("FCm0;storing setting  %d;00#",setupCache.settingNum);
				setToInSettings();							//fill the runtime struct with the settings
				setTempToInSetting(setupCache.settingNum);	//save settings to ram cache
				commandRetrived = 0;
			}

			if (setCmd == 1) {
				print_uart0("FCm0;flashing settings;00#");	//here we write all the settings from ram to flash
				enginesOff(); 								//this is to save some of the flashing cycles.
				led_switch(3);								//we could flash with any retrieved setting but that
				engineStatus = 0;							//just helps abusing the flash cycles so its a manual
															//step to really save to flash
				writeSetup();
				ADCStandstillValues();						//also we reset the ADCOffset
				commandRetrived = 0;
				initFCRuntime();
				ledTest();
			}
		}

		if (commandType == 2) {
			updateAcdRate();
			commandRetrived = 0;
		}
		if (commandType == 3) {
			retriveSetting();
			printSettings();
			commandRetrived = 0;
		}

		if (commandType == 4) {
			if (I2CcmdType == I2CMODE_WRITEADDRESS){
				if (updateYGE == 0) {
					I2C0Mode = I2CMODE_WRITEADDRESS;
					I2C0State = 0;
					updateYGE = 1;
					I2C0Start();
				}
			}

			if (I2CcmdType == I2CMODE_STARTUP_TARGET){
				if (updateYGE == 0) {
					I2C0Stop();
					I2C0Mode = I2CMODE_WRITEADDRESS;
					I2C0State = 0;
					updateYGE = 1;
					I2C0Start();
				}
			}
			commandRetrived = 0;
		}

	}









}
