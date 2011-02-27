/*
	file    i2c.h
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


#ifndef I2C_MASTER_H
#define I2C_MASTER_H

//-----------------------------------------------------------------------------

#define ESC_TYPE_MK 0 //Holger ESCs, note throttle 0..255.
#define ESC_TYPE_YGE 1 //YGE ESCs, note throttle 0..120.
//#define ESC_TYPE_TPRO 2 //TowerPro TWI enabled ESCs, note throttle 0..255.

#define ESC_EXPECTED 			4 		//Number of ESCs expected. Could be set to any value.
#define ESC_ADDRESS_BASE 		0x52 	//Base address for the range of ESCs.
#define ESC_ADDRESS_DEFAULT 	0x88	//Default address for a new ESC.
#define YGE_ADDRESSREG_WRITE 	0x87 	//Address of register to reprogram in the new ESC
#define ESC_STARTUP_COUNT 		64		//should be 32 but to be on the save side due slow YGE startup we send more

#define I2CMODE_STARTUP 		0
#define I2CMODE_FLIGHTMODE 		1
#define I2CMODE_WRITEADDRESS 	2
#define I2CMODE_READADDRESS 	3
#define I2CMODE_STARTUP_TARGET 	4


#define I2C_STARTUP_SEND				0x08 			//A START condition has been transmitted
#define I2C_REPEADET_STARTUP_SEND 		0x10			//A repeated START condition has been transmitted
#define I2C_SLAW_SEND_ACK		 		0x18 			//SLA+W has been transmitted; ACK has been received.
#define I2C_SLAW_SEND_NO_ACK	 		0x20 			//SLA+W has been transmitted; NOT ACK has been received.
#define I2C_DATA_SEND_ACK				0x28 			//Data byte in I2DAT has been transmitted; ACK has been received.
#define I2C_DATA_SEND_NO_ACK			0x30 			//Data byte in I2DAT has been transmitted; NOT ACK has been received.
#define I2C_ARBITRATION_LOST	 		0x38 			//Arbitration lost in SLA+R/W or Data bytes.
#define I2C_SLAR_SEND_ACK		 		0x40 			//SLA+R has been transmitted; ACK has been received.
#define I2C_SLAR_SEND_NO_ACK	 		0x48        	//SLA+R has been transmitted; NOT ACK has been received.
#define I2C_DATA_RECEIVED_SEND_ACK		0x50 			//Data byte has been received; ACK has been returned.
#define I2C_DATA_RECEIVED_SEND_NO_ACK	0x58 			//Data byte has been received; NOT ACK has been returned.



//I2C Control Set Register
#define AA		(1<<2)
#define AA_NOT	(1<<2)
#define SI		(1<<3)
#define STO		(1<<4)
#define STA		(1<<5)
#define I2EN	(1<<6)


#define I2C_IDLE			0
#define I2C_STARTED			1
#define I2C_RESTARTED		2
#define I2C_REPEATED_START	3
#define DATA_ACK			4
#define DATA_NACK			5
#define LASTBYTEREAD		6

volatile unsigned char I2C0State;
volatile unsigned char I2C1State;
volatile unsigned char I2C2State;
volatile unsigned char I2C0Mode;
volatile unsigned char I2C1Mode;
volatile unsigned char I2C2Mode;

volatile signed int hmcXvalue;
volatile signed int hmcYvalue;
volatile signed int hmcZvalue;

unsigned char updateYGE;


int ygeStartI2CCounter;

#define I2CBUFSIZE			0x20

char HMCBuffer[I2CBUFSIZE];

volatile long I2CCmd, I2CMasterState;

volatile long I2CReadLength;
volatile long I2CWriteLength;
volatile long I2CRdIndex;
volatile long I2CWrIndex;


//I2C initialisieren
void I2C0_Init 		(void);
void I2C1_Init 		(void);
void I2C2_Init 		(void);
//Start I2C
void I2C0_Start 		(void);
void I2C1_Start 		(void);
void I2C2_Start 		(void);
void I2C1_ReStart	(void);
//Stop I2C
void I2C0_Stop 		(void);
void I2C1_Stop 		(void);
void I2C2_Stop 		(void);
//1 Byte schreiben
void I2C0_WriteChar 	(char byte);
void I2C1_WriteChar 	(char byte);
void I2C2_WriteChar 	(char byte);


void I2C1InitHMCMode(void);

void __attribute__ ((interrupt("IRQ"))) I2C0_isr_Engine			(void);
void __attribute__ ((interrupt("IRQ"))) I2C0_isr_YGE		(void);
void __attribute__ ((interrupt("IRQ"))) I2C1_isr_DAC		(void);
void __attribute__ ((interrupt("IRQ"))) I2C2_isr			(void);

void __attribute__ ((interrupt("IRQ"))) I2C1_isr_HMC_Mode	(void);


volatile char StatValue;


#endif
