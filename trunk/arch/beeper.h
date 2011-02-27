/*
	file    beeper.h
	author  Werner Oswald
	(c)		Werner Oswald
	mail	osiair@osiworx.com
	date    2010-04-17

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

    2010-04-17 initial version for public release



*/

#ifndef BEEPER_H_
#define BEEPER_H_

#include "lpc23xx.h"
#include "sys_config.h"

#define BEEPER_SET FIO3SET
#define BEEPER_DIR FIO3DIR
#define BEEPER_CLR FIO3CLR
#define BEEPER_PIN FIO3PIN
#define BEEPER_BIT BIT(3) /* P3.3 Pin 2*/

#define BEEPER_OFF   (BEEPER_CLR |= BEEPER_BIT)
#define BEEPER_ON    (BEEPER_SET |= BEEPER_BIT)

void init_beeper(void);
void checkUBat(void);
void beepCountSetting(char setting);

#endif /* BEEPER_H_ */
