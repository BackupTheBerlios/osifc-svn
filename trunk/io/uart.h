/*
	file    uart.h
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

#ifndef UART_H
#define UART_H

#include "arch/sys_config.h"

#define BTDisabled	0
#define BTEnabled	1
#define BTExclusive	2


#define UART0_SUPPORT    1            //non-zero to enable UART0 code
//#define UART1_SUPPORT    1            //non-zero to enable UART1 code
#define UART2_SUPPORT    1            //non-zero to enable UART2 code
#define UART3_SUPPORT    1            //non-zero to enable UART3 code

#define UART0_TX_INT_MODE
#define UART0_RX_INT_MODE
//#define UART1_TX_INT_MODE
//#define UART1_RX_INT_MODE
//#define UART2_TX_INT_MODE
#define UART2_RX_INT_MODE
#define UART3_TX_INT_MODE
#define UART3_RX_INT_MODE

//code is optimized for power of 2 buffer sizes (16, 32, 64, 128, ...)
//NOTE: the buffers are only used if the respective interrupt mode is
//enabled
#define UART0_RX_BUFFER_SIZE 256        //UART0 receive buffer size
#define UART0_TX_BUFFER_SIZE 512       //UART0 transmit buffer size
//#define UART1_RX_BUFFER_SIZE 128        //UART1 receive buffer size
//#define UART1_TX_BUFFER_SIZE 256        //UART1 transmit buffer size
#define UART2_RX_BUFFER_SIZE 256        //UART2 receive buffer size
//#define UART2_TX_BUFFER_SIZE 64        //UART2 transmit buffer size
#define UART3_RX_BUFFER_SIZE 256        //UART3 receive buffer size
#define UART3_TX_BUFFER_SIZE 512        //UART3 transmit buffer size


//use the following macros to determine the 'baud' parameter values
//for uart0Init() and uart1Init()
//CAUTION - 'baud' SHOULD ALWAYS BE A CONSTANT or
//a lot of code will be generated.
#define UART_BAUD(baud) (int)((PCLK / ((baud) * 16.0)) + 0.5)


//Definitions for typical UART 'baud' settings
#define B1200         UART_BAUD(1200)
#define B9600         UART_BAUD(9600)
#define B19200        UART_BAUD(19200)
#define B38400        UART_BAUD(38400)
#define B57600        UART_BAUD(57600)
#define B115200       UART_BAUD(115200)


//Definitions for typical UART 'mode' settings
#define UART_8N1      (char)(ULCR_CHAR_8 + ULCR_PAR_NO   + ULCR_STOP_1)
#define UART_7N1      (char)(ULCR_CHAR_7 + ULCR_PAR_NO   + ULCR_STOP_1)
#define UART_8N2      (char)(ULCR_CHAR_8 + ULCR_PAR_NO   + ULCR_STOP_2)
#define UART_7N2      (char)(ULCR_CHAR_7 + ULCR_PAR_NO   + ULCR_STOP_2)
#define UART_8E1      (char)(ULCR_CHAR_8 + ULCR_PAR_EVEN + ULCR_STOP_1)
#define UART_7E1      (char)(ULCR_CHAR_7 + ULCR_PAR_EVEN + ULCR_STOP_1)
#define UART_8E2      (char)(ULCR_CHAR_8 + ULCR_PAR_EVEN + ULCR_STOP_2)
#define UART_7E2      (char)(ULCR_CHAR_7 + ULCR_PAR_EVEN + ULCR_STOP_2)
#define UART_8O1      (char)(ULCR_CHAR_8 + ULCR_PAR_ODD  + ULCR_STOP_1)
#define UART_7O1      (char)(ULCR_CHAR_7 + ULCR_PAR_ODD  + ULCR_STOP_1)
#define UART_8O2      (char)(ULCR_CHAR_8 + ULCR_PAR_ODD  + ULCR_STOP_2)
#define UART_7O2      (char)(ULCR_CHAR_7 + ULCR_PAR_ODD  + ULCR_STOP_2)


//Definitions for typical UART 'fmode' settings
#define UART_FIFO_OFF (0x00)
#define UART_FIFO_1   (char)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG1)
#define UART_FIFO_4   (char)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG4)
#define UART_FIFO_8   (char)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG8)
#define UART_FIFO_14  (char)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG14)



#if UART0_SUPPORT


void uart0Init(unsigned int baud, unsigned char mode, unsigned char fmode);

int uart0Putch(int ch);

int uart0Putch_block(int ch);


unsigned int uart0Space(void);

char *uart0Puts(char *string);
//same but blocking when no space available (mthomas)
char *uart0Puts_block(char *string);

int uart0Write(char *buffer, unsigned int count);

int uart0TxEmpty(void);

void uart0TxFlush(void);

int uart0Getch(void);

void uart_sendchar(int c);
#endif


#if UART1_SUPPORT

void uart1Init(int baud, char mode, char fmode);


int uart1Putch(int ch);
//same but blocking when no space available (mthomas)
int uart1Putch_block(int ch);


int uart1Space(void);


const char *uart1Puts(const char *string);
//same but blocking when no space available (mthomas)
const char *uart1Puts_block(const char *string);


int uart1Write(const char *buffer, int count);


int uart1TxEmpty(void);


void uart1TxFlush(void);


int uart1Getch(void);


void uart1_sendchar(char c);

#endif

#if UART2_SUPPORT

void uart2Init(unsigned int baud, unsigned char mode, unsigned char fmode);

int uart2Putch(int ch);

int uart2Putch_block(int ch);


unsigned int uart2Space(void);

char *uart2Puts(char *string);
//same but blocking when no space available (mthomas)
char *uart2Puts_block(char *string);

int uart2Write(char *buffer, unsigned int count);

int uart2TxEmpty(void);

void uart2TxFlush(void);

int uart2Getch(void);

void uart2_sendchar(char c);
#endif

#if UART3_SUPPORT

void uart3Init(unsigned int baud, unsigned char mode, unsigned char fmode);

int uart3Putch(int ch);

int uart3Putch_block(int ch);


unsigned int uart3Space(void);

char *uart3Puts(char *string);
//same but blocking when no space available (mthomas)
char *uart3Puts_block(char *string);

int uart3Write(char *buffer, unsigned int count);

int uart3TxEmpty(void);

void uart3TxFlush(void);

int uart3Getch(void);

void uart3_sendchar(char c);
#endif

#endif
