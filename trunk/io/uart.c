/*
	file    uart.c
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
#include "arch/settings.h"
#include "io/uartisr.h"
#include "io/uart.h"


#ifndef USHRT_MAX
#define USHRT_MAX 16
#endif

#if defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE) || \
    defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE) || \
    defined(UART2_TX_INT_MODE) || defined(UART2_RX_INT_MODE) || \
    defined(UART3_TX_INT_MODE) || defined(UART3_RX_INT_MODE)
#endif

#if UART0_SUPPORT
#ifdef UART0_RX_INT_MODE
unsigned char  uart0_rx_buffer[UART0_RX_BUFFER_SIZE];
unsigned int uart0_rx_insert_idx, uart0_rx_extract_idx;
#endif

#ifdef UART0_TX_INT_MODE
unsigned char  uart0_tx_buffer[UART0_TX_BUFFER_SIZE];
unsigned int uart0_tx_insert_idx, uart0_tx_extract_idx;
int      uart0_tx_running;
#endif
#endif

#if UART1_SUPPORT
#ifdef UART1_RX_INT_MODE
unsigned char  uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
unsigned int uart1_rx_insert_idx, uart1_rx_extract_idx;
#endif

#ifdef UART1_TX_INT_MODE
unsigned char  uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
unsigned int uart1_tx_insert_idx, uart1_tx_extract_idx;
int      uart1_tx_running;
#endif
#endif

#if UART2_SUPPORT
#ifdef UART2_RX_INT_MODE
volatile unsigned char  uart2_rx_buffer[UART2_RX_BUFFER_SIZE];
volatile unsigned int uart2_rx_insert_idx, uart2_rx_extract_idx;
#endif

#ifdef UART2_TX_INT_MODE
volatile unsigned char  uart2_tx_buffer[UART2_TX_BUFFER_SIZE];
volatile unsigned int uart2_tx_insert_idx, uart2_tx_extract_idx;
int      uart2_tx_running;
#endif
#endif

#if UART3_SUPPORT
#ifdef UART3_RX_INT_MODE
volatile unsigned char  uart3_rx_buffer[UART3_RX_BUFFER_SIZE];
volatile unsigned int uart3_rx_insert_idx, uart3_rx_extract_idx;
#endif

#ifdef UART3_TX_INT_MODE
volatile unsigned char  uart3_tx_buffer[UART3_TX_BUFFER_SIZE];
volatile unsigned int uart3_tx_insert_idx, uart3_tx_extract_idx;
int      uart3_tx_running;
#endif
#endif

#if UART0_SUPPORT


void uart0Init(unsigned int baud, unsigned char mode, unsigned char fmode)
{

  U0IER = 0x00;                         //disable all interrupts
  U0IIR;                                //clear interrupt ID
  U0RBR;                                //clear receive register
  U0LSR;                                //clear line status register

  //set the baudrate
  U0LCR = ULCR_DLAB_ENABLE;             //select divisor latches
  U0DLL = (unsigned char)baud;                //set for baud low byte
  U0DLM = (unsigned char)(baud >> 8);         //set for baud high byte

  //set the number of characters and other
  //user specified operating parameters
  U0LCR = (mode & ~ULCR_DLAB_ENABLE);
  U0FCR = fmode;

#if defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)
  //initialize the interrupt vector
  VICIntSelect    &= ~VIC_CHAN_TO_MASKLOW(VIC_CHAN_NUM_UART0);
  VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART0);
  VICVectAddr6     = (unsigned long)uart0ISR;
  VICVectPriority6 = 0x0F;
  VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART0);
#if 0
  VICIntSelect &= ~VIC_BIT(VIC_UART0);  //UART0 selected as IRQ
  VICIntEnable = VIC_BIT(VIC_UART0);    //UART0 interrupt enabled
  VICVectCntl0 = VIC_ENABLE | VIC_UART0;
  VICVectAddr0 = (unsigned long)uart0ISR;    //address of the ISR
#endif
#ifdef UART0_TX_INT_MODE
  //initialize the transmit data queue
  uart0_tx_extract_idx = uart0_tx_insert_idx = 0;
  uart0_tx_running = 0;
#endif

#ifdef UART0_RX_INT_MODE
  //initialize the receive data queue
  uart0_rx_extract_idx = uart0_rx_insert_idx = 0;

  //enable receiver interrupts
  U0IER = UIER_RBR;
#endif
#endif
}

void uart_sendchar(int c)
{
	switch (fcSetup.BTMode)
		{
			case BTDisabled:
				uart0Putch(c);
			break;
			case BTEnabled:
				uart0Putch(c);
				uart3Putch(c);
			break;
			case BTExclusive:
				uart3Putch(c);
			break;
		}
}

int uart0Putch(int ch)
{
  unsigned int temp;

  temp = (uart0_tx_insert_idx + 1) % UART0_TX_BUFFER_SIZE;

  if (temp == uart0_tx_extract_idx)
    return -1;                          //no room

  U0IER &= ~UIER_THRE;                  //disable TX interrupts

  //check if in process of sending data
  if (uart0_tx_running)
    {
    //add to queue
    uart0_tx_buffer[uart0_tx_insert_idx] = (unsigned char)ch;
    uart0_tx_insert_idx = temp;
    }
  else
    {
    //set running flag and write to output register
    uart0_tx_running = 1;
    U0THR = (unsigned char)ch;
    }

  U0IER |= UIER_THRE;                   //enable TX interrupts

  VICVectAddr = 0;

  return (unsigned char)ch;
}


unsigned int uart0Space(void)
{
#ifdef UART0_TX_INT_MODE
  int space;

  if ((space = (uart0_tx_extract_idx - uart0_tx_insert_idx)) <= 0)
    space += UART0_TX_BUFFER_SIZE;

  return (unsigned int)(space - 1);
#else
  return USHRT_MAX;
#endif
}

char *uart0Puts(char *string)
{
  register char ch;

  while ((ch = *string) && (uart0Putch(ch) >= 0))
    string++;

  return string;
}

int uart0Write(char *buffer, unsigned int count)
{
#ifdef UART0_TX_INT_MODE
  if (count > uart0Space())
    return -1;
#endif
  while (count && (uart0Putch(*buffer++) >= 0))
    count--;

  return (count ? -2 : 0);
}

int uart0TxEmpty(void)
{
  return (U0LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

void uart0TxFlush(void)
{
#ifdef UART0_TX_INT_MODE
  //unsigned cpsr;

  U0FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo

  //"Empty" the transmit buffer.
  //cpsr = disableIRQ();                  //disable global interrupts
  U0IER &= ~UIER_THRE;                 //disable TX interrupts
  //restoreIRQ(cpsr);                     //restore global interrupts
  uart0_tx_insert_idx = uart0_tx_extract_idx = 0;
#else
  U0FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo
#endif
}

int uart0Getch(void)
{
#ifdef UART0_RX_INT_MODE
  unsigned char ch;

  if (uart0_rx_insert_idx == uart0_rx_extract_idx) //check if character is available
    return -1;

  ch = uart0_rx_buffer[uart0_rx_extract_idx++]; //get character, bump pointer
  uart0_rx_extract_idx %= UART0_RX_BUFFER_SIZE; //limit the pointer
  return ch;
#else
  if (U0LSR & ULSR_RDR)                 //check if character is available
    return U0RBR;                       //return character

  return -1;
#endif
}
#endif

#if UART1_SUPPORT


void uart1Init(unsigned int baud, unsigned char mode, unsigned char fmode)
{

  U1IER = 0x00;                         //disable all interrupts
  U1IIR;                                //clear interrupt ID
  U1RBR;                                //clear receive register
  U1LSR;                                //clear line status register

  //set the baudrate
  U1LCR = ULCR_DLAB_ENABLE;             //select divisor latches
  U1DLL = (unsigned char)baud;                //set for baud low byte
  U1DLM = (unsigned char)(baud >> 8);         //set for baud high byte

  //set the number of characters and other
  //user specified operating parameters
  U1LCR = (mode & ~ULCR_DLAB_ENABLE);
  U1FCR = fmode;

#if defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE)
  //initialize the interrupt vector
  VICIntSelect    &= ~VIC_CHAN_TO_MASKLOW(VIC_CHAN_NUM_UART1);
  VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART1);
  VICVectAddr7     = (unsigned long)uart1ISR;
  VICVectPriority7 = 0x0F;
  VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART1);
#if 0
  VICIntSelect &= ~VIC_BIT(VIC_UART1);  //UART1 selected as IRQ
  VICIntEnable = VIC_BIT(VIC_UART1);    //UART1 interrupt enabled
  VICVectCntl1 = VIC_ENABLE | VIC_UART1;
  VICVectAddr1 = (unsigned long)uart1ISR;    //address of the ISR
#endif


#ifdef UART1_TX_INT_MODE
  uart1_tx_extract_idx = uart1_tx_insert_idx = 0;
  uart1_tx_running = 0;
#endif

#ifdef UART1_RX_INT_MODE
  //initialize data queues
  uart1_rx_extract_idx = uart1_rx_insert_idx = 0;

  //enable receiver interrupts
  U1IER |= UIER_RBR;
#endif
#endif
}


int uart1Putch(int ch)
{
#ifdef UART1_TX_INT_MODE
  unsigned int temp;
  unsigned cpsr;

  temp = (uart1_tx_insert_idx + 1) % UART1_TX_BUFFER_SIZE;

  if (temp == uart1_tx_extract_idx)
    return -1;                          //no room

  cpsr = disableIRQ();                  //disable global interrupts
  U1IER &= ~UIER_THRE;                 //disable TX interrupts
  restoreIRQ(cpsr);                     //restore global interrupts

  //check if in process of sending data
  if (uart1_tx_running)
    {
    //add to queue
    uart1_tx_buffer[uart1_tx_insert_idx] = (unsigned char)ch;
    uart1_tx_insert_idx = temp;
    }
  else
    {
    //set running flag and write to output register
    uart1_tx_running = 1;
    U1THR = (unsigned char)ch;
    }

  cpsr = disableIRQ();                  //disable global interrupts
  U1IER |= UIER_THRE;                   //enable TX interrupts
  restoreIRQ(cpsr);                     //restore global interrupts
#else
  while (!(U1LSR & ULSR_THRE))          //wait for TX buffer to empty
    continue;                           //also either WDOG() or swap()

  U1THR = (unsigned char)ch;
#endif
  return (unsigned char)ch;
}

int uart1Putch_block(int ch)
{
	while ( !uart1Space() ) {
		; //wait for space available
	}
	return uart1Putch(ch);
}

unsigned int uart1Space(void)
{
#ifdef UART1_TX_INT_MODE
  int space;

  if ((space = (uart1_tx_extract_idx - uart1_tx_insert_idx)) <= 0)
    space += UART1_TX_BUFFER_SIZE;

  return (unsigned int)(space - 1);
#else
  return USHRT_MAX;
#endif
}

const char *uart1Puts(const char *string)
{
  register char ch;

  while ((ch = *string) && (uart1Putch(ch) >= 0))
    string++;

  return string;
}

const char *uart1Puts_block(const char *string)
{
  register char ch;

  while ((ch = *string) && (uart1Putch_block(ch) >= 0))
    string++;

  return string;
}

int uart1Write(const char *buffer, unsigned int count)
{
#ifdef UART1_TX_INT_MODE
  if (count > uart1Space())
    return -1;
#endif
  while (count && (uart1Putch(*buffer++) >= 0))
    count--;

  return (count ? -2 : 0);
}

int uart1TxEmpty(void)
{
  return (U1LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

void uart1TxFlush(void)
{
#ifdef UART1_TX_INT_MODE
  unsigned cpsr;

  U1FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo

  //"Empty" the transmit buffer.
  cpsr = disableIRQ();                  //disable global interrupts
  U1IER &= ~UIER_THRE;                 //disable TX interrupts
  restoreIRQ(cpsr);                     //restore global interrupts
  uart1_tx_insert_idx = uart1_tx_extract_idx = 0;
#else
  U1FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo
#endif
}

int uart1Getch(void)
{
#ifdef UART1_RX_INT_MODE
  unsigned char ch;

  if (uart1_rx_insert_idx == uart1_rx_extract_idx) //check if character is available
    return -1;

  ch = uart1_rx_buffer[uart1_rx_extract_idx++]; //get character, bump pointer
  uart1_rx_extract_idx %= UART1_RX_BUFFER_SIZE; //limit the pointer
  return ch;
#else
  if (U1LSR & ULSR_RDR)                 //check if character is available
    return U1RBR;                       //return character

  return -1;
#endif
}

#endif

#if UART2_SUPPORT

void uart2Init(unsigned int baud, unsigned char mode, unsigned char fmode)
{
  U2IER = 0x00;                         //disable all interrupts
  U2IIR;                                //clear interrupt ID
  U2RBR;                                //clear receive register
  U2LSR;                                //clear line status register

  //set the baudrate
  U2LCR = ULCR_DLAB_ENABLE;             //select divisor latches
  U2DLL = (unsigned char)baud;                //set for baud low byte
  U2DLM = (unsigned char)(baud >> 8);         //set for baud high byte

  //set the number of characters and other
  //user specified operating parameters
  U2LCR = (mode & ~ULCR_DLAB_ENABLE);
  U2FCR = fmode;

#if defined(UART2_TX_INT_MODE) || defined(UART2_RX_INT_MODE)
  //initialize the interrupt vector
  VICIntSelect    &= ~VIC_CHAN_TO_MASKLOW(VIC_CHAN_NUM_UART2);
  VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART2);
  VICVectAddr28     = (unsigned long)uart2ISR;
  VICVectPriority28 = 0x0F;
  VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART2);
#if 0
  VICIntSelect &= ~VIC_BIT(VIC_UART2);  //UART2 selected as IRQ
  VICIntEnable = VIC_BIT(VIC_UART2);    //UART2 interrupt enabled
  VICVectCntl0 = VIC_ENABLE | VIC_UART2;
  VICVectAddr0 = (unsigned long)uart2ISR;    //address of the ISR
#endif
#ifdef UART2_TX_INT_MODE
  //initialize the transmit data queue
  uart2_tx_extract_idx = uart2_tx_insert_idx = 0;
  uart2_tx_running = 0;
#endif

#ifdef UART2_RX_INT_MODE
  //initialize the receive data queue
  uart2_rx_extract_idx = uart2_rx_insert_idx = 0;

  //enable receiver interrupts
  U2IER = UIER_RBR;
#endif
#endif
}

void uart2_sendchar(char c)
{
  uart2Putch(c);
}

int uart2Putch(int ch)
{
#ifdef UART2_TX_INT_MODE
  unsigned int temp;
  unsigned cpsr;

  temp = (uart2_tx_insert_idx + 1) % UART2_TX_BUFFER_SIZE;

  if (temp == uart2_tx_extract_idx)
    return -1;                          //no room

  cpsr = disableIRQ();                  //disable global interrupts
  U2IER &= ~UIER_THRE;                  //disable TX interrupts
  restoreIRQ(cpsr);                     //restore global interrupts

  //check if in process of sending data
  if (uart2_tx_running)
    {
    //add to queue
    uart2_tx_buffer[uart2_tx_insert_idx] = (unsigned char)ch;
    uart2_tx_insert_idx = temp;
    }
  else
    {
    //set running flag and write to output register
    uart2_tx_running = 1;
    U2THR = (unsigned char)ch;
    }

  cpsr = disableIRQ();                  //disable global interrupts
  U2IER |= UIER_THRE;                   //enable TX interrupts
  restoreIRQ(cpsr);                     //restore global interrupts
#else
  while (!(U2LSR & ULSR_THRE))          //wait for TX buffer to empty
    continue;                           //also either WDOG() or swap()

  U2THR = (unsigned char)ch;
#endif
  return (unsigned char)ch;
}

unsigned int uart2Space(void)
{
#ifdef UART2_TX_INT_MODE
  int space;

  if ((space = (uart2_tx_extract_idx - uart2_tx_insert_idx)) <= 0)
    space += UART2_TX_BUFFER_SIZE;

  return (unsigned int)(space - 1);
#else
  return USHRT_MAX;
#endif
}

char *uart2Puts(char *string)
{
  register char ch;

  while ((ch = *string) && (uart2Putch(ch) >= 0))
    string++;

  return string;
}

int uart2Write(char *buffer, unsigned int count)
{
#ifdef UART2_TX_INT_MODE
  if (count > uart2Space())
    return -1;
#endif
  while (count && (uart2Putch(*buffer++) >= 0))
    count--;

  return (count ? -2 : 0);
}

int uart2TxEmpty(void)
{
  return (U2LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

void uart2TxFlush(void)
{
#ifdef UART2_TX_INT_MODE
  unsigned cpsr;

  U2FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo

  //"Empty" the transmit buffer.
  cpsr = disableIRQ();                  //disable global interrupts
  U2IER &= ~UIER_THRE;                 //disable TX interrupts
  restoreIRQ(cpsr);                     //restore global interrupts
  uart2_tx_insert_idx = uart2_tx_extract_idx = 0;
#else
  U2FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo
#endif
}

int uart2Getch(void)
{
#ifdef UART2_RX_INT_MODE
  unsigned char ch;

  if (uart2_rx_insert_idx == uart2_rx_extract_idx) //check if character is available
    return -1;

  ch = uart2_rx_buffer[uart2_rx_extract_idx++]; //get character, bump pointer
  uart2_rx_extract_idx %= UART2_RX_BUFFER_SIZE; //limit the pointer
  return ch;
#else
  if (U2LSR & ULSR_RDR)                 //check if character is available
    return U2RBR;                       //return character

  return -1;
#endif
}

#endif

#if UART3_SUPPORT

void uart3Init(unsigned int baud, unsigned char mode, unsigned char fmode)
{

  U3IER = 0x00;                         //disable all interrupts
  U3IIR;                                //clear interrupt ID
  U3RBR;                                //clear receive register
  U3LSR;                                //clear line status register

  //set the baudrate
  U3LCR = ULCR_DLAB_ENABLE;             //select divisor latches
  U3DLL = (unsigned char)baud;                //set for baud low byte
  U3DLM = (unsigned char)(baud >> 8);         //set for baud high byte

  //set the number of characters and other
  //user specified operating parameters
  U3LCR = (mode & ~ULCR_DLAB_ENABLE);
  U3FCR = fmode;

#if defined(UART3_TX_INT_MODE) || defined(UART3_RX_INT_MODE)
  //initialize the interrupt vector
  VICIntSelect    &= VIC_CHAN_TO_MASKLOW(VIC_CHAN_NUM_UART3);
  VICIntEnClr      = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART3);
  VICVectAddr29     = (unsigned long)uart3ISR;
  VICVectPriority29 = 0x0F;
  VICIntEnable     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_UART3);
#ifdef UART3_TX_INT_MODE
  //initialize the transmit data queue
  uart3_tx_extract_idx = uart3_tx_insert_idx = 0;
  uart3_tx_running = 0;
#endif

#ifdef UART3_RX_INT_MODE
  //initialize the receive data queue
  uart3_rx_extract_idx = uart3_rx_insert_idx = 0;

  //enable receiver interrupts
  U3IER = UIER_RBR;
#endif
#endif
}

void uart3_sendchar(char c)
{
  uart3Putch(c);
}

int uart3Putch(int ch)
{

  unsigned int temp;

  temp = (uart3_tx_insert_idx + 1) % UART3_TX_BUFFER_SIZE;

  if (temp == uart3_tx_extract_idx)
    return -1;                          //no room
  U3IER &= ~UIER_THRE;                  //disable TX interrupts
  //check if in process of sending data
  if (uart3_tx_running)
    {
    //add to queue
    uart3_tx_buffer[uart3_tx_insert_idx] = (unsigned char)ch;
    uart3_tx_insert_idx = temp;
    }
  else
    {
    //set running flag and write to output register
    uart3_tx_running = 1;
    U3THR = (unsigned char)ch;
    }

   U3IER |= UIER_THRE;                   //enable TX interrupts
   VICVectAddr = 0;

  return (unsigned char)ch;
}

unsigned int uart3Space(void)
{
#ifdef UART3_TX_INT_MODE
  int space;

  if ((space = (uart3_tx_extract_idx - uart3_tx_insert_idx)) <= 0)
    space += UART3_TX_BUFFER_SIZE;

  return (unsigned int)(space - 1);
#else
  return USHRT_MAX;
#endif
}

char *uart3Puts(char *string)
{
  register char ch;

  while ((ch = *string) && (uart3Putch(ch) >= 0))
    string++;

  return string;
}

int uart3Write(char *buffer, unsigned int count)
{
#ifdef UART3_TX_INT_MODE
  if (count > uart3Space())
    return -1;
#endif
  while (count && (uart3Putch(*buffer++) >= 0))
    count--;

  return (count ? -2 : 0);
}

int uart3TxEmpty(void)
{
  return (U3LSR & (ULSR_THRE | ULSR_TEMT)) == (ULSR_THRE | ULSR_TEMT);
}

void uart3TxFlush(void)
{
#ifdef UART3_TX_INT_MODE


  U3FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo

  //"Empty" the transmit buffer.
  U3IER &= ~UIER_THRE;                 //disable TX interrupts
  uart3_tx_insert_idx = uart3_tx_extract_idx = 0;
#else
  U3FCR |= UFCR_TX_FIFO_RESET;          //clear the TX fifo
#endif
}

int uart3Getch(void)
{
#ifdef UART3_RX_INT_MODE
  unsigned char ch;

  if (uart3_rx_insert_idx == uart3_rx_extract_idx) //check if character is available
    return -1;

  ch = uart3_rx_buffer[uart3_rx_extract_idx++]; //get character, bump pointer
  uart3_rx_extract_idx %= UART3_RX_BUFFER_SIZE; //limit the pointer
  return ch;
#else
  if (U3LSR & ULSR_RDR)                 //check if character is available
    return U3RBR;                       //return character

  return -1;
#endif
}
#endif



