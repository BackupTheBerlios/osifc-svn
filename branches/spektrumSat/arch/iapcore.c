/*
	file    iapCore.c
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

//this is the IAP core code which is used to store the settings
//there is also potential to build in a own firmware update at
//runtime without the need of the bootloader but thats something
//for you guys to contribute ;)
//deliver something waiting for the hex file and I put something
//into the base station tool to send in the hex, deal?

//dump

//0 4 0X0000 0000 - 0X0000 0FFF
//1 4 0X0000 1000 - 0X0000 1FFF
//2 4 0X0000 2000 - 0X0000 2FFF
//3 4 0X0000 3000 - 0X0000 3FFF
//4 4 0X0000 4000 - 0X0000 4FFF
//5 4 0X0000 5000 - 0X0000 5FFF
//6 4 0X0000 6000 - 0X0000 6FFF
//7 4 0X0000 7000 - 0X0000 7FFF
//8 32 0x0000 8000 - 0X0000 FFFF
//9 32 0x0001 0000 - 0X0001 7FFF
//10 (0x0A) 32 0x0001 8000 - 0X0001 FFFF
//11 (0x0B) 32 0x0002 0000 - 0X0002 7FFF
//12 (0x0C) 32 0x0002 8000 - 0X0002 FFFF
//13 (0x0D) 32 0x0003 0000 - 0X0003 7FFF
//14 (0X0E) 32 0x0003 8000 - 0X0003 FFFF
//15 (0x0F) 32 0x0004 0000 - 0X0004 7FFF
//16 (0x10) 32 0x0004 8000 - 0X0004 FFFF
//17 (0x11) 32 0x0005 0000 - 0X0005 7FFF
//18 (0x12) 32 0x0005 8000 - 0X0005 FFFF
//19 (0x13) 32 0x0006 0000 - 0X0006 7FFF
//20 (0x14) 32 0x0006 8000 - 0X0006 FFFF
//21 (0x15) 32 0x0007 0000 - 0X0007 7FFF
//22 (0x16) 4 0x0007 8000 - 0X0007 8FFF
//23 (0x17) 4 0x0007 9000 - 0X0007 9FFF
//24 (0x18) 4 0x0007 A000 - 0X0007 AFFF here we are storing the settings at the end we take the whole 16k from here on
//25 (0x19) 4 0x0007 B000 - 0X0007 BFFF
//26 (0x1A) 4 0x0007 C000 - 0X0007 CFFF
//27 (0x1B) 4 0x0007 D000 - 0X0007 DFFF here I will add the firmware upload code so we can do firmware updates from withion the tools no flash magig needed.

//to store the setting we use the following process:

//store the setting into the named setting channel
//compare saved settings to settings in ram
//if compare gives us a full match we signal settings have been saved
//for a start we will have 16 slots for settings
//each slot will be 256 Bytes long. this is kind of a
//system requirement as the LPC2378 stores in 256 Byte blocks
//there are still 47 such slots left for future use
//the IAP is done in thumb mode there for we need to switch on
//interworking in the makefile
//one 256 Byte slot will be taken for the firmware loader

#include "iapCore.h"
#include "viclowlevel.h"
#include "sys_config.h"
#include "settings.h"
#include "../io/output.h"
#include "led.h"



unsigned int SectorAddressRange[28][2] __attribute__ ((__aligned__))
                                    = { {0x00000000, 0x00000FFF},
{0x00001000, 0x00001FFF}, {0x00002000, 0x00002FFF}, {0x00003000, 0x00003FFF},
{0x00004000, 0x00004FFF}, {0x00005000, 0x00005FFF}, {0x00006000, 0x00006FFF},
{0x00007000, 0x00007FFF}, {0x00008000, 0x0000FFFF}, {0x00010000, 0x00017FFF},
{0x00018000, 0x0001FFFF}, {0x00020000, 0x00027FFF}, {0x00028000, 0x0002FFFF},
{0x00030000, 0x00037FFF}, {0x00038000, 0x0003FFFF}, {0x00040000, 0x00047FFF},
{0x00048000, 0x0004FFFF}, {0x00050000, 0x00057FFF}, {0x00058000, 0x0005FFFF},
{0x00060000, 0x00067FFF}, {0x00068000, 0x0006FFFF}, {0x00070000, 0x00077FFF},
{0x00078000, 0x00078FFF}, {0x00079000, 0x00079FFF}, {0x0007A000, 0x0007AFFF},
{0x0007B000, 0x0007BFFF}, {0x0007C000, 0x0007CFFF}, {0x0007D000, 0x0007DFFF}};



//here we create a matrix for easy access to all the different start addresses.
//toDo calculate the address out of slot and channel

unsigned long slotChannels[4][16] __attribute__ ((__aligned__))
                              = {
		{SLOT_00_SET_00_ADDR,SLOT_00_SET_01_ADDR,SLOT_00_SET_02_ADDR,SLOT_00_SET_03_ADDR,
				SLOT_00_SET_04_ADDR,SLOT_00_SET_05_ADDR,SLOT_00_SET_06_ADDR,SLOT_00_SET_07_ADDR,
				SLOT_00_SET_08_ADDR,SLOT_00_SET_09_ADDR,SLOT_00_SET_10_ADDR,SLOT_00_SET_11_ADDR,
				SLOT_00_SET_12_ADDR,SLOT_00_SET_13_ADDR,SLOT_00_SET_14_ADDR,SLOT_00_SET_15_ADDR}

		, {SLOT_01_SET_00_ADDR,SLOT_01_SET_01_ADDR,SLOT_01_SET_02_ADDR,SLOT_01_SET_03_ADDR,
				SLOT_01_SET_04_ADDR,SLOT_01_SET_05_ADDR,SLOT_01_SET_06_ADDR,SLOT_01_SET_07_ADDR,
				SLOT_01_SET_08_ADDR,SLOT_01_SET_09_ADDR,SLOT_01_SET_10_ADDR,SLOT_01_SET_11_ADDR,
				SLOT_01_SET_12_ADDR,SLOT_01_SET_13_ADDR,SLOT_01_SET_14_ADDR,SLOT_01_SET_15_ADDR}

		, {SLOT_02_SET_00_ADDR,SLOT_02_SET_01_ADDR,SLOT_02_SET_02_ADDR,SLOT_02_SET_03_ADDR,
				SLOT_02_SET_04_ADDR,SLOT_02_SET_05_ADDR,SLOT_02_SET_06_ADDR,SLOT_02_SET_07_ADDR,
				SLOT_02_SET_08_ADDR,SLOT_02_SET_09_ADDR,SLOT_02_SET_10_ADDR,SLOT_02_SET_11_ADDR,
				SLOT_02_SET_12_ADDR,SLOT_02_SET_13_ADDR,SLOT_02_SET_14_ADDR,SLOT_02_SET_15_ADDR}

		, {SLOT_03_SET_00_ADDR,SLOT_03_SET_01_ADDR,SLOT_03_SET_02_ADDR,SLOT_03_SET_03_ADDR,
				SLOT_03_SET_04_ADDR,SLOT_03_SET_05_ADDR,SLOT_03_SET_06_ADDR,SLOT_03_SET_07_ADDR,
				SLOT_03_SET_08_ADDR,SLOT_03_SET_09_ADDR,SLOT_03_SET_10_ADDR,SLOT_03_SET_11_ADDR,
				SLOT_03_SET_12_ADDR,SLOT_03_SET_13_ADDR,SLOT_03_SET_14_ADDR,SLOT_03_SET_15_ADDR}
};

unsigned char settingSlots[4] = {SETTINGSLOT_00,SETTINGSLOT_01,SETTINGSLOT_02,SETTINGSLOT_03};

IAP iap_entry = (IAP) IAP_LOCATION;


int PrepareSector4Write (unsigned int sector_start,unsigned int sector_end)
{
	//prepare sector 4 write
	command[0] = PrepareSectors4Write;
	command[1] = sector_start;
	command[2] = sector_end;
	iap_entry (command, result);

	while ((result[0] == BUSY));
	return result[0];
}



int EraseSector (unsigned long start_sect_no, unsigned long stop_sect_no, unsigned long CCLK_freq)
{
	int status;
	//prepare sector 4 write

	status=PrepareSector4Write(start_sect_no,stop_sect_no);
	//erase prepared sector
	if (status==CMD_SUCCESS)
	{
		command[0] = EraseSectors;
		command[1] = start_sect_no;
		command[2] = stop_sect_no;
		command[3] = CCLK_freq;
		iap_entry (command, result);

		checkIapResult(result[0]);
	}
	return result[0];
}


unsigned long getTempAddr(char setNum)
{
	switch (setNum)
	{
		case 0:
			setupStore.settingsStore = (INCOMINGSETTINGS)setupCacheT0;
			incomingSettingsAddrTemp = (volatile int*)&setupStore;
		break;
		case 1:
			setupStore.settingsStore = (INCOMINGSETTINGS)setupCacheT1;
			incomingSettingsAddrTemp = (volatile int*)&setupStore;
		break;
		case 2:
			setupStore.settingsStore = (INCOMINGSETTINGS)setupCacheT2;
			incomingSettingsAddrTemp = (volatile int*)&setupStore;
		break;
		case 3:
			setupStore.settingsStore = (INCOMINGSETTINGS)setupCacheT3;
			incomingSettingsAddrTemp = (volatile int*)&setupStore;
		break;
		case 4:
			setupStore.settingsStore = (INCOMINGSETTINGS)setupCacheT4;
			incomingSettingsAddrTemp = (volatile int*)&setupStore;
		break;
		case 5:
			setupStore.settingsStore = (INCOMINGSETTINGS)setupCacheT5;
			incomingSettingsAddrTemp = (volatile int*)&setupStore;
		break;
	}
	return (int)incomingSettingsAddrTemp;
}

unsigned long *dstFlashAddr;
unsigned long *srcRamAddr;

void WriteEEPROM(char slot)
{
	unsigned char sector;
	sector = settingSlots[(int)slot];

	int i = 0;
	ledOFF();
	//disable IRQ while writing to flash
	disableIRQ();

	EraseSector(sector,sector,CCLK / 1000);

	while (i <= 5 ) {

		dstFlashAddr = (unsigned long*)slotChannels[(int)slot][i];
		srcRamAddr = (unsigned long*)getTempAddr(i);

		command[0] = PrepareSectors4Write;
		command[1] = sector;
		command[2] = sector;

		iap_entry (command, result);
		while (result[0] == BUSY);

		//copy ram to flash
		command[0] = RAM2Flash;  			//IAP Copy Ram to flash operation
		command[1] = (unsigned long)dstFlashAddr;   	//address of sector [slot][channel]
		command[2] = (unsigned long)srcRamAddr;   		//fcSetup location
		command[3] = 256;					//how many bytes to write (256) can be 512 1024 4096
		command[4] = CCLK / 1000;  			//clock speed

		iap_entry (command, result);
		while (result[0] == BUSY);
		led_switch(i);
		i++;
	}
	enableIRQ();
	checkIapResult(result[0]);
}

char *srcFlashAddr;
volatile char *incomingSettingsAddr;

long value;
void ReadEEPROM(char slot, char ReadChannel)
{
	unsigned int sector;
	srcFlashAddr = (char*)slotChannels[(int)slot][(int)ReadChannel];
	incomingSettingsAddr = (volatile char*)&setupStore;

	volatile char *address;
	address = (volatile char*)incomingSettingsAddr;
	sector = settingSlots[(int)slot];

	//toDo find a better way to do this
	int i = 0;
	while (i++ < 250) {
		*incomingSettingsAddr++ = *srcFlashAddr++;
	}

}


