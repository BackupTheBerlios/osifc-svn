/*
	file    i2c.c
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

#include "i2c.h"
#include "engines.h"
#include "output.h"
#include "compass.h"

#include "../arch/lpc23xx.h"
#include "../arch/clock-arch.h"
#include "../arch/sys_config.h"
#include "../arch/settings.h"
#include "../arch/printf_p.h"
#include "../arch/analog.h"


#include "../interface/command.h"

unsigned int I2C0Status;


long I2CCLR = 0x28;

//-------------------------------------------------------------------------------
//Start I2C
void I2C0_Start(void)
{
	I2C0CONCLR = I2CCLR; //Clear all Flags
	I2C0CONSET |= STA;
}
void I2C1_Start(void)
{
	I2C1CONCLR = I2CCLR; //Clear all Flags
	I2C1CONSET |= STA;
}
void I2C1_ReStart(void)
{
	I2C1CONCLR = I2CCLR; //Clear all Flags
	I2C1CONSET = STA;
}
void I2C2_Start(void)
{
	I2C2CONCLR = I2CCLR; //Clear all Flags
	I2C2CONSET |= STA;
}

//-------------------------------------------------------------------------------
//Stop I2C
void I2C0_Stop(void)
{
	I2C0CONCLR = I2CCLR; //Clear all Flags
	I2C0CONSET |= STO;
	while( I2C0CONSET & STO );
}
void I2C1_Stop(void)
{
	I2C1CONCLR = I2CCLR; //Clear all Flags
	I2C1CONSET |= STO;
	while( I2C1CONSET & STO );
}
void I2C2_Stop(void)
{
	I2C2CONCLR = I2CCLR; //Clear all Flags
	I2C2CONSET |= STO;
	while( I2C2CONSET & STO );
}

//-------------------------------------------------------------------------------
//I2C write byte
void I2C0_WriteChar(char payload)
{
	I2C0DAT = payload;
	I2C0CONCLR = I2CCLR;
}
void I2C1_WriteChar(char payload)
{
	I2C1DAT = payload;
	I2C1CONCLR = I2CCLR;
}
void I2C2_WriteChar(char payload)
{
	I2C2DAT = payload;
	I2C2CONCLR = I2CCLR;
}


void I2C1ReadChar(char payload);
void I2C1ReadChar(char payload)
{
	//I2C1CONCLR = 0x04;
	//I2C2CONSET |= AA;
	I2C1DAT = payload;

}
//-------------------------------------------------------------------------------
//Start I2C

void __attribute__ ((interrupt("IRQ"))) I2C0_isr_Engine(void)
{
    switch (I2C0State++)
        {
        case 0:
        		I2C0_WriteChar((fcSetup.escBaseAdr +(engine*2))); //&  0x52  fcSetup.escBaseAdr
        break;
        case 1:
        		I2C0_WriteChar(PWMEngOut[engine++]);
        break;
        case 2:
                I2C0_Stop();
                if (engine==4)
				{
					engine = 0;
				}
				I2C0State = 0;
        break;
        }
	VICVectAddr = 0;
}


unsigned int tmp;
unsigned char I2CGas;

void __attribute__ ((interrupt("IRQ"))) I2C0_isr_YGE(void)
{
	switch (I2C0Mode)
	{
		case I2CMODE_STARTUP:
	    switch (I2C0State++)
	        {
			case 0:
				I2C0_WriteChar((char)(fcSetup.escBaseAdr+(engine++*2))); 	//	 & 0x7f
			break;
			case 1:
				I2C0_WriteChar(((char)0) & 0x7f );
				ygeStartI2CCounter++;
		    break;
			case 2:
				I2C0_Stop();
				if (ygeStartI2CCounter == 290) {
			    	I2C0Mode = I2CMODE_FLIGHTMODE;
			    	I2C0State = 0;
			    	I2C0_Start();
			    } else {
			    	if (engine==ESC_EXPECTED)
					{
						engine = 0;
					}
			    	I2C0State = 0;
			    	I2C0_Start();
			    }
			break;

	        }
	    break;
	    case I2CMODE_FLIGHTMODE:

		switch (I2C0State++)
	        {
			case 0:
				/*if (I2C0STAT != I2C_STARTUP_SEND) {
					I2C0State = 0;
			    	I2C0_Start();
				} else {
				*/
				I2C0_WriteChar((char)fcSetup.escBaseAdr+engine*2);
				//}
			break;
			case 1:
				I2CGas = (char)(PWMEngOut[engine++]*120/255);
				I2C0_WriteChar((char)(I2CGas & 0x7f));
		    break;
			case 2:
				I2C0_Stop();
				if (engine==ESC_EXPECTED)
				{
					engine = 0;
				}
				I2C0State = 0;
				I2C0_Start();
			break;
	        }
	    break;
	    //write a register by now only slave adress:
	    //S SLA+W 1 REG DATA St

	    case I2CMODE_WRITEADDRESS:
	    	if (updateYGE == 1) {
	    		switch (I2C0State++)
		        {

	    		case 0:

	    			I2C0_WriteChar((char)ygeActualAdress);
	    		break;
	    		case 1:

	    			I2C0_WriteChar((char)YGE_ADDRESSREG_WRITE);
			    break;
	    		case 2:

					I2C0_WriteChar((char)ygeTargetAdress);
			    break;
			    case 3:
			    	I2C0_Stop();
			    	I2C0Mode = I2CMODE_FLIGHTMODE;
			    	I2C0_Start();
			    	print_uart0("FCm0;have send YGE Change Address command change from %X to %X;00#",(char)ygeActualAdress,(char)ygeTargetAdress);
			    break;
			    case 4:
			    	I2C0State = 4;
				break;
		        }
	    	}
	    break;
    	case I2CMODE_STARTUP_TARGET:
	    switch (I2C0State++)
	        {
			case 0:
				I2C0_WriteChar((char)ygeTargetAdress);
			break;
			case 1:
				I2C0_WriteChar(((char)0) & 0x7f );
				ygeStartI2CCounter++;
		    break;
			case 2:
				I2C0_Stop();
			    if (ygeStartI2CCounter == 35) {
			    	//ygeStartI2CCounter = 0;

			    	I2C0Mode = I2CMODE_FLIGHTMODE;
			    	I2C0State = 0;
			    	I2C0_Start();
			    } else {
			    	I2C0_Start();
			    	I2C0State = 0;
			    }
			break;
			case 3:
				I2C0State = 3;
	    	break;
	        }
	    break;
	    //read a adress
	    //S SLA+W 1 REG Sr SLA+R data St
	    case I2CMODE_READADDRESS:
	       	switch (I2C0State++)
		        {
	    		case 0:
	    			I2C0_WriteChar((char)ygeActualAdress);
	    		break;
	    		case 2:
					I2C0_WriteChar((char)ygeTargetAdress);
			    break;
			    case 3:
			    	I2C0_Start();
			    break;
			    case 4:
					I2C0_WriteChar((char)ygeActualAdress);
			    break;
	    		case 5:
					I2C0_WriteChar((char)255);
			    break;
			    case 6:
			    	I2C0_Stop();
				break;
				case 7:
			    	I2C0State = 7;
				break;
		        }
	    break;
	}
	VICVectAddr = 0;
}

static int DACChannel[4] = {0x10,0x12,0x14,0x16};
static int DACChannelAssignment[4] = {ADC_NICK,ADC_ROLL,ADC_PITCH,AIRPRESSURE};

void __attribute__ ((interrupt("IRQ"))) I2C1_isr_DAC(void)
{
	switch (I2C1State++)
	{
		case 0:
			I2C1_WriteChar((char)0x98); // Address of the DAC
		break;
		case 1:
			I2C1_WriteChar(DACChannel[I2C1Mode]); // Update Channel I2C1Mode
		break;
		case 2:
			I2C1_WriteChar((char)ADC_offset[DACChannelAssignment[I2C1Mode]]); // Value from ADC_offset at position DACChannelAssignment[I2C1Mode]
		break;
		case 3:
			I2C1_WriteChar((char)0x80); // Value
		break;
		case 4:
			I2C1Mode++;
			I2C1State = 0;
			I2C1_Stop();
			I2C1_Start();
		break;
		case 5:
			//print_uart0("FCm0;I2C Mode %d;00#",I2C1Mode);
			I2C1Mode = 0;
			I2C1_Stop();
			I2C1State = 5;
		break;
	}
	if(I2C1Mode >=4)
	{
		I2C1State = 5;
	}
	VICVectAddr = 0;
}



void __attribute__ ((interrupt("IRQ"))) I2C1_isr_HMC_Mode(void)
{

	 StatValue = I2C1STAT;

	 switch ( StatValue )
	  {
		case 0x08:			/* A Start condition is issued. */
			I2C1DAT = HMCBuffer[0];
			I2C1CONCLR = (SI | STA);
		break;

		case 0x10:			// A repeated started is issued
			I2C1CONCLR = (SI | STA);
		break;
		case 0x18:			/* Regardless, it's a ACK */
			I2C1DAT = HMCBuffer[1+I2CWrIndex];
			I2CWrIndex++;
			I2C1CONCLR = SI;
		break;

		case 0x28:	/* Data byte has been transmitted, regardless ACK or NACK */
		case 0x30:
			if ( I2CWrIndex != I2CWriteLength )
			{
				  I2C1DAT = HMCBuffer[1+I2CWrIndex]; /* this should be the last one */
				  I2CWrIndex++;
			}
			 else
			{
				  I2C1CONSET = STO;
			}
			I2C1CONCLR = SI;
		break;

		case 0x40:	/* Master Receive, SLA_R has been sent */
			I2C1CONSET = AA;	/* assert ACK */
			I2C1CONCLR = SI;
		break;

		case 0x50:	/* Data byte has been received, regardless following ACK or NACK */
		case 0x58:
			HMCBuffer[1+I2CRdIndex] = I2C1DAT;
			//print_uart0("%X		%X	;00#",StatValue,HMCBuffer[1+I2CRdIndex]);
			I2CRdIndex++;
			if ( I2CRdIndex <= I2CReadLength )
			{
				 if ( I2CRdIndex == I2CReadLength ) {
					 I2C1CONCLR = AA_NOT;/* assert NACK after last Byte is received */

				 } else {
					 I2C1CONSET = AA;	/* assert ACK after Byte is received */

				 }
			}
			else
			{
				switch ( HMCBuffer[0] )
				{
					case 0x3D:
					HMC_runtime.X_axis = (((signed char)HMCBuffer[1] << 8) | (signed char)HMCBuffer[2]);
					HMC_runtime.Y_axis = (((signed char)HMCBuffer[3] << 8) | (signed char)HMCBuffer[4]);
					HMC_runtime.Z_axis = (((signed char)HMCBuffer[5] << 8) | (signed char)HMCBuffer[6]);
					break;
					case 0x33:
					//	print_uart0(".");
					HMC_runtime.heading = (((signed char)HMCBuffer[1] << 8) | (signed char)HMCBuffer[2]);
					HMC_runtime.pitch 	= (((signed char)HMCBuffer[3] << 8) | (signed char)HMCBuffer[4]);
					HMC_runtime.roll 	= (((signed char)HMCBuffer[5] << 8) | (signed char)HMCBuffer[6]);
					HMC6343Read = 0;
					break;
				}

				I2CRdIndex = 0;
				I2CWrIndex = 0;
				I2C1CONCLR = 0x28;
				I2C1CONSET = STO;
			}
			I2C1CONCLR = SI;
		break;

		case 0x20:		/* regardless, it's a NACK */
		case 0x48:
			I2C1CONCLR = SI;
		break;

		case 0x38:		/* Arbitration lost, in this example, we don't
						deal with multiple master situation */
		default:
			I2C1CONCLR = SI;
		break;
	  }
	  VICVectAddr = 0;		/* Acknowledge Interrupt */
}


void __attribute__ ((interrupt("IRQ"))) I2C2_isr(void)
{
	I2C2CONCLR = 0x28;
	VICVectAddr = 0;
}


void I2C0_Init(void)
{
	//I2C0 on Pin: P0.27,P0.28
		PINSEL1 |= (1<<22)|(1<<24);
		switch (fcSetup.escType) //fcSetup.escType
	    {
		case ESC_TYPE_MK:
			I2C0SCLH			= 0x0080;// 0x02FF;			//Holger Controller work with equal high and low times
			I2C0SCLL			= 0x0080;
			VICVectAddr9    	= (unsigned long)I2C0_isr_Engine;
		break;
		case ESC_TYPE_YGE:
			I2C0SCLH			= 0x0040;					//YGE likes to have very short high times
			I2C0SCLL			= 0x0380;					//but much longer low times
			VICVectAddr9    	= (unsigned long)I2C0_isr_YGE;
		break;
	    }

		VICIntSelect    		&= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C0);
		VICIntEnClr      		= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C0);
		VICVectPriority9 		= 0x0E;
		VICIntEnable     		= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C0);

		engines_Off();

		I2C0CONCLR 				= 0x28; //Clear all Flags
		I2C0CONSET 				|= I2EN; //I2C Enable
}


void I2C1_Init(void)
{
	//SDA1 P0.0(66), SCL1 P0.1(67)
	I2C1State = 0;
	I2C1Mode = 0;

	PINSEL0 			|= (1<<0)|(1<<1);
	PINSEL0 			|= (1<<2)|(1<<3);

	I2C1SCLH			= 0x02FF;
	I2C1SCLL			= 0x02FF;
	VICVectAddr19   	= (unsigned long)I2C1_isr_DAC;

	VICIntSelect    	&= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C1);
	VICIntEnClr      	= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C1);
	VICVectPriority19 	= 0x0f;
	VICIntEnable    	= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C1);

	I2C1CONCLR 			= 0x28; //Clear all Flags
	I2C1CONSET 			|= I2EN; //I2C Enable

}

void I2C2_Init(void)
{
	I2C2State = 0;

	PINSEL0 			|= (0<<20)|(1<<21);
	PINSEL0 			|= (1<<22)|(0<<23);

	//PINMODE0 |= (1 << 21) | (1 << 23);

	I2C2SCLH			= 0x0080;
	I2C2SCLL			= 0x0080;

	VICVectAddr30    	= (unsigned long)I2C2_isr;
	VICIntSelect    	&= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C2);
	VICIntEnClr      	= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C2);
	VICVectPriority30 	= 0x0F;
	VICIntEnable     	= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C2);

	I2C2CONCLR 			= 0x28; //Clear all Flags
	I2C2CONSET 			|= I2EN; //I2C Enable

}


void I2C1InitHMCMode(void)
{
	//SDA1 P0.0(66), SCL1 P0.1(67)
	I2C1State = 0;
	I2C1Mode = 0;

	PINSEL0 			|= (1<<0)|(1<<1);
	PINSEL0 			|= (1<<2)|(1<<3);

	I2C1SCLH			= 0x0080;
	I2C1SCLL			= 0x0080;
	VICVectAddr19   	= (unsigned long)I2C1_isr_HMC_Mode;

	VICIntSelect    	&= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C1);
	VICIntEnClr      	= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C1);
	VICVectPriority19 	= 0x0F;
	VICIntEnable    	= VIC_CHAN_TO_MASK(VIC_CHAN_NUM_I2C1);

	I2C1CONCLR 			= 0x28; //Clear all Flags
	I2C1CONSET 			= I2EN;
}


