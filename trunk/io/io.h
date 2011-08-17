/*
	file    io.h
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


#ifndef IO_H
#define IO_H

#include "../arch/lpc23xx.h"

#define IO0_SET FIO1SET
#define IO0_DIR FIO1DIR
#define IO0_CLR FIO1CLR
#define IO0_PIN FIO1PIN
#define IO0_BIT BIT(16) /* P1.16 */
#define IO0_NBIT NBIT(16) /* P1.16 */

#define IO1_SET FIO4SET
#define IO1_DIR FIO4DIR
#define IO1_CLR FIO4CLR
#define IO1_PIN FIO4PIN
#define IO1_BIT BIT(24) /* P4.24 */
#define IO1_NBIT NBIT(24) /* P4.24 */

#define IO2_SET FIO1SET
#define IO2_DIR FIO1DIR
#define IO2_CLR FIO1CLR
#define IO2_PIN FIO1PIN
#define IO2_BIT BIT(10) /* P1.10 */
#define IO2_NBIT NBIT(10) /* P1.10 */

#define IO3_SET FIO1SET
#define IO3_DIR FIO1DIR
#define IO3_CLR FIO1CLR
#define IO3_PIN FIO1PIN
#define IO3_BIT BIT(9) /* P1.9 */
#define IO3_NBIT NBIT(9) /* P1.9 */

#define IO4_SET FIO1SET
#define IO4_DIR FIO1DIR
#define IO4_CLR FIO1CLR
#define IO4_PIN FIO1PIN
#define IO4_BIT BIT(4) /* P1.4 */
#define IO4_NBIT NBIT(4) /* P1.4 */

#define IO0_OFF   IO1_CLR |= IO0_BIT;
#define IO0_ON    IO1_SET |= IO0_BIT
#define IO0_FLASH IO1_PIN ^= IO0_BIT;

#define IO1_OFF   IO1_CLR |= IO1_BIT;
#define IO1_ON    IO1_SET |= IO1_BIT
#define IO1_FLASH IO1_PIN ^= IO1_BIT;

#define IO2_OFF   IO1_CLR |= IO2_BIT;
#define IO2_ON    IO1_SET |= IO2_BIT
#define IO2_FLASH IO1_PIN ^= IO2_BIT;

#define IO3_OFF   IO1_CLR |= IO3_BIT;
#define IO3_ON    IO1_SET |= IO3_BIT
#define IO3_FLASH IO1_PIN ^= IO3_BIT;

#define IO4_OFF   IO1_CLR |= IO4_BIT;
#define IO4_ON    IO1_SET |= IO4_BIT
#define IO4_FLASH IO1_PIN ^= IO4_BIT;

void IO_Init0(void);
void IO_Init1(void);
void IO_Init2(void);
void IO_Init3(void);
void IO_Init4(void);

#endif /*IO_H*/
