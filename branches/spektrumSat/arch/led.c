/*
	file    led.c
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

#include "led.h"


volatile int flip = 0;


volatile char led1switch = 0;
volatile char led2switch = 0;
volatile char led3switch = 0;
volatile char led4switch = 0;

void led_switch(char ledNum) {
	switch (ledNum)
	{
	case 1:
		if (led1switch == 1) {
			LED1_OFF;
			led1switch = !led1switch;
		} else {
			LED1_ON;
			led1switch = !led1switch;
		}
	break;
	case 2:
		if (led2switch == 1) {
			LED2_OFF;
			led2switch = !led2switch;
		} else {
			LED2_ON;
			led2switch = !led2switch;
		}
	break;
	case 3:
		if (led3switch == 1) {
			LED3_OFF;
			led3switch = !led3switch;
		} else {
			LED3_ON;
			led3switch = !led3switch;
		}
	break;
	case 4:
		if (led4switch == 1) {
			LED4_OFF;
			led4switch = !led4switch;
		} else {
			LED4_ON;
			led4switch = !led4switch;
		}
	break;
	}
}

void ledOFF(void) {
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	led1switch = 0;
	led2switch = 0;
	led3switch = 0;
	led4switch = 0;
}





void ledDelay(int delay)
{
	unsigned int tmp;
	for(tmp = 0;tmp<delay*1000;tmp++){asm("nop");};
}


void ledTest(void)
{
	unsigned int tmp;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	LED1_ON;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	LED1_OFF;
	LED2_ON;
	LED3_OFF;
	LED4_OFF;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	LED1_OFF;
	LED2_OFF;
	LED3_ON;
	LED4_OFF;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_ON;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	LED1_OFF;
	LED2_OFF;
	LED3_ON;
	LED4_OFF;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	LED1_OFF;
	LED2_ON;
	LED3_OFF;
	LED4_OFF;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	LED1_ON;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	for(tmp = 0;tmp<200000;tmp++){asm("nop");};
	ledOFF();
}
