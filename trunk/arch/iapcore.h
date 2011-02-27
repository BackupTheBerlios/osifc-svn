/*
	file    iapCore.h
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

#define PrepareSectors4Write 50
#define RAM2Flash 51
#define EraseSectors 52
#define BlankCheckSectors 53
#define ReadPartID 54
#define ReadBootCodeVersion 55
#define Compare 56
#define ReinvokeISP 57

#define CMD_SUCCESS 		0 						//Command is executed successfully.
#define INVALID_COMMAND 	1						//Invalid command.
#define SRC_ADDR_ERROR 		2 						//Source address is not on a word boundary.
#define DST_ADDR_ERROR 		3 						//Destination address is not on a correct boundary.
#define SRC_ADDR_NOT_MAPPED 4 						//Source address is not mapped in the memory map.
#define DST_ADDR_NOT_MAPPED 5 						//Destination address is not mapped inthe memory
#define COUNT_ERROR 		6 						//Byte count is not multiple of 4 or is not a permitted value.
#define INVALID_SECTOR 		7 						//Sector number is invalid.
#define SECTOR_NOT_BLANK 	8 						//Sector is not blank.
#define SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 9 	//Command to prepare sector for write operation was not executed.
#define COMPARE_ERROR 		10 						//Source and destination data is not same.
#define BUSY 				11
#define IAP_LOCATION 		0x7FFFFFF1

#define SETTINGSLOT_00 24
#define SETTINGSLOT_01 25
#define SETTINGSLOT_02 26
#define SETTINGSLOT_03 27

//here we define the addresses where to store the setup data
#define SLOT_00_SET_00_ADDR 0x0007A000
#define SLOT_00_SET_01_ADDR 0x0007A100
#define SLOT_00_SET_02_ADDR 0x0007A200
#define SLOT_00_SET_03_ADDR 0x0007A300
#define SLOT_00_SET_04_ADDR 0x0007A400
#define SLOT_00_SET_05_ADDR 0x0007A500
#define SLOT_00_SET_06_ADDR 0x0007A600
#define SLOT_00_SET_07_ADDR 0x0007A700
#define SLOT_00_SET_08_ADDR 0x0007A800
#define SLOT_00_SET_09_ADDR 0x0007A900
#define SLOT_00_SET_10_ADDR 0x0007AA00
#define SLOT_00_SET_11_ADDR 0x0007AB00
#define SLOT_00_SET_12_ADDR 0x0007AC00
#define SLOT_00_SET_13_ADDR 0x0007AD00
#define SLOT_00_SET_14_ADDR 0x0007AE00
#define SLOT_00_SET_15_ADDR 0x0007AF00

#define SLOT_01_SET_00_ADDR 0x0007B000
#define SLOT_01_SET_01_ADDR 0x0007B100
#define SLOT_01_SET_02_ADDR 0x0007B200
#define SLOT_01_SET_03_ADDR 0x0007B300
#define SLOT_01_SET_04_ADDR 0x0007B400
#define SLOT_01_SET_05_ADDR 0x0007B500
#define SLOT_01_SET_06_ADDR 0x0007B600
#define SLOT_01_SET_07_ADDR 0x0007B700
#define SLOT_01_SET_08_ADDR 0x0007B800
#define SLOT_01_SET_09_ADDR 0x0007B900
#define SLOT_01_SET_10_ADDR 0x0007BA00
#define SLOT_01_SET_11_ADDR 0x0007BB00
#define SLOT_01_SET_12_ADDR 0x0007BC00
#define SLOT_01_SET_13_ADDR 0x0007BD00
#define SLOT_01_SET_14_ADDR 0x0007BE00
#define SLOT_01_SET_15_ADDR 0x0007BF00


#define SLOT_02_SET_00_ADDR 0x0007C000
#define SLOT_02_SET_01_ADDR 0x0007C100
#define SLOT_02_SET_02_ADDR 0x0007C200
#define SLOT_02_SET_03_ADDR 0x0007C300
#define SLOT_02_SET_04_ADDR 0x0007C400
#define SLOT_02_SET_05_ADDR 0x0007C500
#define SLOT_02_SET_06_ADDR 0x0007C600
#define SLOT_02_SET_07_ADDR 0x0007C700
#define SLOT_02_SET_08_ADDR 0x0007C800
#define SLOT_02_SET_09_ADDR 0x0007C900
#define SLOT_02_SET_10_ADDR 0x0007CA00
#define SLOT_02_SET_11_ADDR 0x0007CB00
#define SLOT_02_SET_12_ADDR 0x0007CC00
#define SLOT_02_SET_13_ADDR 0x0007CD00
#define SLOT_02_SET_14_ADDR 0x0007CE00
#define SLOT_02_SET_15_ADDR 0x0007CF00

#define SLOT_03_SET_00_ADDR 0x0007D000
#define SLOT_03_SET_01_ADDR 0x0007D100
#define SLOT_03_SET_02_ADDR 0x0007D200
#define SLOT_03_SET_03_ADDR 0x0007D300
#define SLOT_03_SET_04_ADDR 0x0007D400
#define SLOT_03_SET_05_ADDR 0x0007D500
#define SLOT_03_SET_06_ADDR 0x0007D600
#define SLOT_03_SET_07_ADDR 0x0007D700
#define SLOT_03_SET_08_ADDR 0x0007D800
#define SLOT_03_SET_09_ADDR 0x0007D900
#define SLOT_03_SET_10_ADDR 0x0007DA00
#define SLOT_03_SET_11_ADDR 0x0007DB00
#define SLOT_03_SET_12_ADDR 0x0007DC00
#define SLOT_03_SET_13_ADDR 0x0007DD00
#define SLOT_03_SET_14_ADDR 0x0007DE00
#define SLOT_03_SET_15_ADDR 0x0007DF00

typedef void (*IAP)(unsigned int [],unsigned int[]);


unsigned int command[5];
unsigned int result[2];

volatile int *incomingSettingsAddrTemp;
volatile int *incomingSettingsAddrIAP;


void WriteEEPROM (char slot);
void ReadEEPROM(char slot, char ReadChannel);

unsigned long getTempAddr(char setNum);
int EraseSector (unsigned long start_sect_no, unsigned long stop_sect_no, unsigned long CCLK_freq);
int PrepareSector4Write (unsigned int sector_start,unsigned int sector_end);

int GetBootCodeVersion (void);
int GetPartID (void);
//int EraseSector (unsigned long start_sect_no, unsigned long stop_sect_no, unsigned long CCLK_freq);







