/*
	file    uartISR.h
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

#ifndef UART_ISR_H
#define UART_ISR_H
#include "uart.h"

#if UART0_SUPPORT
#if defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)
void uart0ISR(void);
#endif //UART0_TX_INT_MODE || UART0_RX_INT_MODE
#endif //UART0_SUPPORT


#if UART1_SUPPORT
#if defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE)
void uart1ISR(void);
#endif //UART1_TX_INT_MODE || UART1_RX_INT_MODE
#endif //UART1_SUPPORT

#if UART2_SUPPORT
#if defined(UART2_TX_INT_MODE) || defined(UART2_RX_INT_MODE)
void uart2ISR(void);
#endif //UART2_TX_INT_MODE || UART2_RX_INT_MODE
#endif //UART2_SUPPORT

#if UART3_SUPPORT
#if defined(UART3_TX_INT_MODE) || defined(UART3_RX_INT_MODE)
void uart3ISR(void);
#endif //UART3_TX_INT_MODE || UART3_RX_INT_MODE
#endif //UART3_SUPPORT


#endif
