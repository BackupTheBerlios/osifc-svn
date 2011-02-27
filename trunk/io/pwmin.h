/*
	file    pwmin.h
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


#ifndef pwmin_H
#define pwmin_H



#define pwmin0_SET 	FIO2SET
#define pwmin0_DIR 	FIO2DIR
#define pwmin0_CLR 	FIO2CLR
#define pwmin0_PIN 	FIO2PIN
#define pwmin0_BIT 	BIT(6)   /* P2.6 */
#define pwmin0_NBIT NBIT(6) /* P2.6 */
#define pwmin8_BIT 	BIT(7)   /* P2.7 */


#define PWMIN_PIN_0		0x10
#define PWMIN_PIN_1		0x20
#define PWMIN_PIN_2		0x40
#define PWMIN_PIN_3		0x8000
#define PWMIN_PIN_4		0x10000
#define PWMIN_PIN_5		0x20000
#define PWMIN_PIN_6		0x40000
#define PWMIN_PIN_7		PWMIN_PIN_1
#define PWMIN_PIN_8		0x80
#define PWMIN_PIN_9		PWMIN_PIN_2			//this one is used for receiving the multichannel signal
											//it could be used for single channel PWM as well but isnt yet
											//don't worry me reusing values as they are used on different channels
											//10 to 12 are missing as there are no pins. you may add pin 2.6 if unused


// The DRDY Pin to select which one is used P2.2 (0x04) or P2.7 (0x80)
#define DRDY_PIN		0x04				// PWMIN_PIN_8 if used

//PWM SingleChannel states

#define	PWM_Raising		0
#define	PWM_Falling		1

#define	PWM_MC			0
#define	PWM_SC			1


volatile unsigned int PWM_THROTTLE;
volatile unsigned int PWM_R;
volatile unsigned int PWM_N;
volatile unsigned int PWM_G;
volatile unsigned int PWM_POTI1;
volatile unsigned int PWM_POTI2;
volatile unsigned int PWM_POTI3;
volatile unsigned int PWM_user4;
volatile unsigned int PWM_user5;
volatile unsigned int PWM_user6;
volatile unsigned int PWM_user7;
volatile unsigned int PWM_user8;



volatile unsigned char PWM_chan_count;		//to count the actual channel when in multichannel mode
volatile unsigned int PWM_valid;			//used to judge if PWM readout still delivers trustable results
											//if not parashoot should be released and engines stoped toDo implement parashoot

volatile signed int PWM_mux[12];			//hardcoded to be Maximum 12 if more is needed update here
volatile signed int PWM_count[12];
volatile signed int PWM_channel[12];
volatile unsigned int OldTimerVal;			//to get the Difference betewen interupts we store the old value this saves us from reseting timer values
volatile unsigned int TimerVal;				//actual timer value to "freeze" the value for processing
volatile unsigned int PWM_SC_Temp[10]; 		//to store the OldTimer values when using multiport PWMin

void pwmin_SC_ISR(void);
void pwmin_MC_ISR(void);
void initPWMvars (void);

#endif /*pwmin_H*/
