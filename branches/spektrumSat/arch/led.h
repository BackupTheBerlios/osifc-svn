/*
	file    led.h
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

#ifndef LED_H
#define LED_H


#include "lpc23xx.h"
#include "sys_config.h"

#define LED1_SET FIO4SET
#define LED1_DIR FIO4DIR
#define LED1_CLR FIO4CLR
#define LED1_PIN FIO4PIN
#define LED1_BIT BIT(1) /* P4.1 */

#define LED2_SET FIO1SET
#define LED2_DIR FIO1DIR
#define LED2_CLR FIO1CLR
#define LED2_PIN FIO1PIN
#define LED2_BIT BIT(25) /* P1.25 */

#define LED3_SET FIO4SET
#define LED3_DIR FIO4DIR
#define LED3_CLR FIO4CLR
#define LED3_PIN FIO4PIN
#define LED3_BIT BIT(2) /* P4.2 */

#define LED4_SET FIO1SET
#define LED4_DIR FIO1DIR
#define LED4_CLR FIO1CLR
#define LED4_PIN FIO1PIN
#define LED4_BIT BIT(27) /* P1.27 */

//-------------------------------------------------------------------------------
//Hardware Led
#define LED1_OFF   (LED1_CLR |= LED1_BIT)
#define LED1_ON    (LED1_SET |= LED1_BIT)
#define LED2_OFF   (LED2_CLR |= LED2_BIT)
#define LED2_ON    (LED2_SET |= LED2_BIT)
#define LED3_OFF   (LED3_CLR |= LED3_BIT)
#define LED3_ON    (LED3_SET |= LED3_BIT)
#define LED4_OFF   (LED4_CLR |= LED4_BIT)
#define LED4_ON    (LED4_SET |= LED4_BIT)

void ledDelay(int delay);
void led_switch(char ledNum);
void ledTest(void);
void ledOFF(void);
#endif /*LED_H*/
