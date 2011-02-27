/*
	file    io.c
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

#include "arch/sys_config.h"
#include "io/io.h"




void ioInit0(void)
{
	IO0_DIR |= IO0_BIT;
}
void ioInit1(void)
{
	IO1_DIR |= IO1_BIT;
}
void ioInit2(void)
{
	IO2_DIR |= IO2_BIT;
}
void ioInit3(void)
{
	IO3_DIR |= IO3_BIT;
}
void ioInit4(void)
{
	IO4_DIR |= IO4_BIT;
}
