/*
	file    uartISR.c
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


#include "uartisr.h"

#include "printf_p.h"
#include "uart.h"
#include "command.h"


#if UART0_SUPPORT

#ifdef UART0_RX_INT_MODE
extern char  uart0_rx_buffer[UART0_RX_BUFFER_SIZE];
extern int uart0_rx_insert_idx, uart0_rx_extract_idx;
#endif //UART0_RX_INT_MODE

#ifdef UART0_TX_INT_MODE
extern char  uart0_tx_buffer[UART0_TX_BUFFER_SIZE];
extern int uart0_tx_insert_idx, uart0_tx_extract_idx;
extern int      uart0_tx_running;
#endif //UART0_TX_INT_MODE

#if defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)




void __attribute__ ((interrupt("IRQ"))) uart0ISR(void)
{
	volatile char iid;
	volatile char temp;
  //loop until not more interrupt sources
  while (((iid = U0IIR) & UIIR_NO_INT) == 0)
    {
    //identify & process the highest priority interrupt
    switch (iid & UIIR_ID_MASK)
      {
      case UIIR_RLS_INT:                //Receive Line Status
        U0LSR;                          //read LSR to clear
        break;

#ifdef UART0_RX_INT_MODE
      case UIIR_CTI_INT:                //Character Timeout Indicator
      case UIIR_RDA_INT:                //Receive Data Available
        do
          {
          temp = U0RBR;
         // print_uart0("%2x",temp);
          checkSerialIn(temp);
         }
        while (U0LSR & ULSR_RDR);

        break;
#endif

#ifdef UART0_TX_INT_MODE
      case UIIR_THRE_INT:               //Transmit Holding Register Empty
        while (U0LSR & ULSR_THRE)
          {
          //check if more data to send
          if (uart0_tx_insert_idx != uart0_tx_extract_idx)
            {
            U0THR = uart0_tx_buffer[uart0_tx_extract_idx++];
            uart0_tx_extract_idx %= UART0_TX_BUFFER_SIZE;
            }
          else
            {
            //no
            uart0_tx_running = 0;       //clear running flag
            break;
            }
          }

        break;
#endif //UART0_TX_INT_MODE

      default:                          //Unknown
        U0LSR;
        U0RBR;
        break;
      }
    }

  VICVectAddr = 0;             //clear this interrupt from the VIC

}
#endif //defined(UART0_TX_INT_MODE) || defined(UART0_RX_INT_MODE)
#endif //UART0_SUPPORT


#if UART1_SUPPORT

#ifdef UART1_RX_INT_MODE
extern char  uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
extern int uart1_rx_insert_idx, uart1_rx_extract_idx;
#endif //UART1_RX_INT_MODE

#ifdef UART1_TX_INT_MODE
extern char  uart1_tx_buffer[UART1_TX_BUFFER_SIZE];
extern int uart1_tx_insert_idx, uart1_tx_extract_idx;
extern int      uart1_tx_running;
#endif //UART1_TX_INT_MODE

#if defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE)

void uart1ISR(void)
{
  char iid;

  //perform proper ISR entry so thumb-interwork works properly
  ISR_ENTRY();

  //loop until not more interrupt sources
  while (((iid = U1IIR) & UIIR_NO_INT) == 0)
    {
    //identify & process the highest priority interrupt
    switch (iid & UIIR_ID_MASK)
      {
      case UIIR_RLS_INT:                //Receive Line Status
        U1LSR;                          //read LSR to clear
        break;

#ifdef UART1_RX_INT_MODE
      case UIIR_CTI_INT:                //Character Timeout Indicator
      case UIIR_RDA_INT:                //Receive Data Available
        do
          {
          int temp;

          //calc next insert index & store character
          temp = (uart1_rx_insert_idx + 1) % UART1_RX_BUFFER_SIZE;
          uart1_rx_buffer[uart1_rx_insert_idx] = U1RBR;

          //check for more room in queue
          if (temp != uart1_rx_extract_idx)
            uart1_rx_insert_idx = temp; //update insert index
          }
        while (U1LSR & ULSR_RDR);

        break;
#endif

#ifdef UART1_TX_INT_MODE
      case UIIR_THRE_INT:               //Transmit Holding Register Empty
        while (U1LSR & ULSR_THRE)
          {
          //check if more data to send
          if (uart1_tx_insert_idx != uart1_tx_extract_idx)
            {
            U1THR = uart1_tx_buffer[uart1_tx_extract_idx++];
            uart1_tx_extract_idx %= UART1_TX_BUFFER_SIZE;
            }
          else
            {
            //no
            uart1_tx_running = 0;       //clear running flag
            break;
            }
          }

        break;
#endif //UART1_TX_INT_MODE

      case UIIR_MS_INT:                 //MODEM Status
        U1MSR;                          //read MSR to clear
        break;

      default:                          //Unknown
        U1LSR;
        U1RBR;
        U1MSR;
        break;
      }
    }

  VICVectAddr = 0;             //clear this interrupt from the VIC
  ISR_EXIT();                           //recover registers and return
}
#endif //defined(UART1_TX_INT_MODE) || defined(UART1_RX_INT_MODE)
#endif //UART1_SUPPORT

#if UART2_SUPPORT

#ifdef UART2_RX_INT_MODE
extern char  uart2_rx_buffer[UART2_RX_BUFFER_SIZE];
extern int uart2_rx_insert_idx, uart2_rx_extract_idx;
#endif //UART2_RX_INT_MODE

#ifdef UART2_TX_INT_MODE
extern char  uart2_tx_buffer[UART2_TX_BUFFER_SIZE];
extern int uart2_tx_insert_idx, uart2_tx_extract_idx;
extern int      uart2_tx_running;
#endif //UART2_TX_INT_MODE

#if defined(UART2_TX_INT_MODE) || defined(UART2_RX_INT_MODE)


void __attribute__ ((interrupt("IRQ"))) uart2ISR(void)
{
	volatile char iid;



  //loop until not more interrupt sources
  while (((iid = U2IIR) & UIIR_NO_INT) == 0)
    {
    //identify & process the highest priority interrupt
    switch (iid & UIIR_ID_MASK)
      {
      case UIIR_RLS_INT:                //Receive Line Status
        U2LSR;                          //read LSR to clear
        break;

#ifdef UART2_RX_INT_MODE
      case UIIR_CTI_INT:                //Character Timeout Indicator
      case UIIR_RDA_INT:                //Receive Data Available
        do
          {
        volatile char temp;

          //calc next insert index & store character
          temp = U2RBR;

          //no queue since we just take it into usage and dont use it later..
          //this is good as long as we see comunication is not harming mission critical systems
          }
        while (U2LSR & ULSR_RDR);

        break;
#endif

#ifdef UART2_TX_INT_MODE
      case UIIR_THRE_INT:               //Transmit Holding Register Empty
        while (U2LSR & ULSR_THRE)
          {
          //check if more data to send
          if (uart2_tx_insert_idx != uart2_tx_extract_idx)
            {
            U2THR = uart2_tx_buffer[uart2_tx_extract_idx++];
            uart2_tx_extract_idx %= UART2_TX_BUFFER_SIZE;
            }
          else
            {
            //no
            uart2_tx_running = 0;       //clear running flag
            break;
            }
          }

        break;
#endif //UART2_TX_INT_MODE

      default:                          //Unknown
        U2LSR;
        U2RBR;
        break;
      }
    }

  VICVectAddr = 0;             //clear this interrupt from the VIC

}
#endif //defined(UART2_TX_INT_MODE) || defined(UART2_RX_INT_MODE)

#endif //UART2_SUPPORT

#if UART3_SUPPORT

#ifdef UART3_RX_INT_MODE
extern char  uart3_rx_buffer[UART3_RX_BUFFER_SIZE];
extern int uart3_rx_insert_idx, uart3_rx_extract_idx;
#endif //UART3_RX_INT_MODE

#ifdef UART3_TX_INT_MODE
extern char  uart3_tx_buffer[UART3_TX_BUFFER_SIZE];
extern int uart3_tx_insert_idx, uart3_tx_extract_idx;
extern int      uart3_tx_running;
#endif //UART3_TX_INT_MODE

#if defined(UART3_TX_INT_MODE) || defined(UART3_RX_INT_MODE)


void __attribute__ ((interrupt("IRQ"))) uart3ISR(void)
{
	volatile char iid;

  //loop until not more interrupt sources
  while (((iid = U3IIR) & UIIR_NO_INT) == 0)
    {
    //identify & process the highest priority interrupt
    switch (iid & UIIR_ID_MASK)
      {
      case UIIR_RLS_INT:                //Receive Line Status
        U3LSR;                          //read LSR to clear
        break;

#ifdef UART3_RX_INT_MODE
      case UIIR_CTI_INT:                //Character Timeout Indicator
      case UIIR_RDA_INT:                //Receive Data Available
        do
          {
        volatile char temp;
          temp = U3RBR;
         // print_uart3("%2x",temp);
          checkSerialIn(temp);
         }
        while (U3LSR & ULSR_RDR);

        break;
#endif

#ifdef UART3_TX_INT_MODE
      case UIIR_THRE_INT:               //Transmit Holding Register Empty
        while (U3LSR & ULSR_THRE)
          {
          //check if more data to send
          if (uart3_tx_insert_idx != uart3_tx_extract_idx)
            {
            U3THR = uart3_tx_buffer[uart3_tx_extract_idx++];
            uart3_tx_extract_idx %= UART3_TX_BUFFER_SIZE;
            }
          else
            {
            //no
            uart3_tx_running = 0;       //clear running flag
            break;
            }
          }

        break;
#endif //UART3_TX_INT_MODE

      default:                          //Unknown
        U3LSR;
        U3RBR;
        break;
      }
    }

  VICVectAddr = 0;             //clear this interrupt from the VIC

}
#endif //defined(UART3_TX_INT_MODE) || defined(UART3_RX_INT_MODE)
#endif //UART3_SUPPORT
