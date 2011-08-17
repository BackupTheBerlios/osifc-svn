/*
	file    rcUI.h.c
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

#ifndef RCUI_H_
#define RCUI_H_

#define FULL_LEFT	100
#define FULL_RIGHT	-100
#define FULL_UP		100
#define FULL_DOWN	-100

#define ENGINE_OFF	0
#define ENGINE_ON	1

void offFlightRcUI(void);
void inFlightRcUI(void);

#endif /*RCUI_H_*/
