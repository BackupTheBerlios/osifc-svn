/*
	file    init.h
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

#ifndef INIT_H
#define INIT_H

#define CRP 0x11223344

#define CRP1  0x12345678
#define CRP2  0x87654321
#define CRP3  0x43218765
#define NOCRP CRP

void init_VIC(void);
void reset_GPIO( void );
void init_PLL(void);
void init_Powr(void);
void init_LowLevelSystems(void);
void init_ADC_HW(void);
void init_LED(void);
void init_PWMinChannels(void);
void init_sspi0(void);
void init_spi1(void);
void init_SDcard(void);
void init_PWM(void);
void init_uart(void);
void init_RTC(void);
void init_MM3(void);
void init_system(void);

#endif /*INIT_H*/
