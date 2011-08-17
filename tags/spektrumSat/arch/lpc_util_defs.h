/*
	file    LPC_UTIL_DEFS.h
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


#ifndef LPC_UTILDEFS_H
#define LPC_UTILDEFS_H

#include "sys_config.h"

///////////////////////////////////////////////////////////////////////////////
//CLKSRC defines
#define CLKSRC_INT_RC    (0x00)
#define CLKSRC_MAIN_OSC  (0x01)
#define CLKSRC_MIN_RTC   (0x02)

///////////////////////////////////////////////////////////////////////////////
//SCS defines
#define SCS_GPIOM        (1 << 0)
#define SCS_EMC_RST_DIS  (1 << 1)
#define SCS_MCIPWR       (1 << 3)
#define SCS_OSCRANGE     (1 << 4)
#define SCS_OSCEN        (1 << 5)
#define SCS_OSCSTAT      (1 << 6)

///////////////////////////////////////////////////////////////////////////////
//MAM defines
#define MAMCR_OFF     0
#define MAMCR_PART    1
#define MAMCR_FULL    2

#define MAMCR_MODE MAMCR_PART
//#define MAMCR_MODE MAMCR_FULL


///////////////////////////////////////////////////////////////////////////////
//MEMMAP defines
#define MEMMAP_BBLK   0                 //Interrupt Vectors in Boot Block
#define MEMMAP_FLASH  1                 //Interrupt Vectors in Flash
#define MEMMAP_SRAM   2                 //Interrupt Vectors in SRAM

///////////////////////////////////////////////////////////////////////////////
//PLL defines & computations
//Compute the value of PLL_DIV and test range validity

//PLLCON Register Bit Definitions
#define PLLCON_PLLE   (1 << 0)          //PLL Enable
#define PLLCON_PLLC   (1 << 1)          //PLL Connect

//PLLSTAT Register Bit Definitions
#define PLLSTAT_PLLE  (1 << 24)
#define PLLSTAT_PLLC  (1 << 25)
#define PLLSTAT_PLOCK (1 << 26)

//PLLCFG Register Bit Definitions
#define PLLCFG_MSEL   ((PLL_MUL - 1) << 0) //PLL Multiplier
//#define PLLCFG_PSEL   ((PLL_DIV - 1) << 5) //PLL Divider
#define PLLCFG_NSEL   ((PLL_DIV - 1) << 16UL) //PLL Divider

//PLLSTAT Register Bit Definitions
#define PLLSTAT_LOCK  (1 << 10)         //PLL Lock Status Bit


///////////////////////////////////////////////////////////////////////////////
//UART defines
#define U0_TX_PINSEL_REG  PINSEL0
#define U0_TX_LOW     (1 << 4)  /* PINSEL0 Value for UART0 TX */
#define U0_TX_HIGH    (0 << 5)  /* PINSEL0 Mask  for UART0 TX */
#define U0_RX_PINSEL_REG  PINSEL0
#define U0_RX_LOW     (1 << 6)  /* PINSEL0 Value for UART0 RX */
#define U0_RX_HIGH    (0 << 7)  /* PINSEL0 Mask  for UART0 RX */
#define U0_TX_PINMODE_REG PINMODE0
#define U0_TX_PINMODE_LOW	(0 << 4)
#define U0_TX_PINMODE_HIGH	(1 << 5)
#define U0_RX_PINMODE_REG PINMODE0
#define U0_RX_PINMODE_LOW	(0 << 6)
#define U0_RX_PINMODE_HIGH	(1 << 7)

#define U1_TX_PINSEL_REG  PINSEL4
#define U1_TX_LOW     (0 << 0)  /* PINSEL0 Value for UART1 TX */
#define U1_TX_HIGH    (1 << 1)  /* PINSEL0 Mask  for UART1 TX */
#define U1_RX_PINSEL_REG  PINSEL4
#define U1_RX_LOW     (0 << 2)  /* PINSEL1 Value for UART1 RX */
#define U1_RX_HIGH    (1 << 3)  /* PINSEL1 Mask  for UART1 RX */
#define U1_TX_PINMODE_REG PINMODE4
#define U1_TX_PINMODE_LOW	(1 << 0)
#define U1_TX_PINMODE_HIGH	(0 << 1)
#define U1_RX_PINMODE_REG PINMODE4
#define U1_RX_PINMODE_LOW	(1 << 2)
#define U1_RX_PINMODE_HIGH	(0 << 3)


#define U2_TX_PINSEL_REG  PINSEL4
#define U2_TX_LOW     (0 << 16)  /* PINSEL4 Value for UART2 TX */
#define U2_TX_HIGH    (1 << 17)  /* PINSEL4 Mask  for UART2 TX */
#define U2_RX_PINSEL_REG  PINSEL4
#define U2_RX_LOW     (0 << 18)  /* PINSEL4 Value for UART2 RX */
#define U2_RX_HIGH    (1 << 19)  /* PINSEL4 Mask  for UART2 RX */
#define U2_TX_PINMODE_REG PINMODE4
#define U2_TX_PINMODE_LOW	(0 << 16)
#define U2_TX_PINMODE_HIGH	(1 << 17)
#define U2_RX_PINMODE_REG PINMODE4
#define U2_RX_PINMODE_LOW	(0 << 18)
#define U2_RX_PINMODE_HIGH	(1 << 19)

#define U3_TX_PINSEL_REG  PINSEL9
#define U3_TX_LOW     (1 << 24)  /* PINSEL9 Value for UART3 TX */
#define U3_TX_HIGH    (1 << 25)  /* PINSEL9 Mask  for UART3 TX */
#define U3_RX_PINSEL_REG  PINSEL9
#define U3_RX_LOW     (1 << 26)  /* PINSEL9 Value for UART3 RX */
#define U3_RX_HIGH    (1 << 27)  /* PINSEL9 Mask  for UART3 RX */
#define U3_TX_PINMODE_REG PINMODE4
#define U3_TX_PINMODE_LOW	(0 << 24)
#define U3_TX_PINMODE_HIGH	(1 << 25)
#define U3_RX_PINMODE_REG PINMODE4
#define U3_RX_PINMODE_LOW	(0 << 26)
#define U3_RX_PINMODE_HIGH	(1 << 27)


//Interrupt Enable Register bit definitions
#define UIER_RBR           (1 << 0)    //(UIER_ERBFI) Enable Receive Data Available Interrupt
#define UIER_THRE          (1 << 1)    //(UIER_ETBEI) Enable Transmit Holding Register Empty Interrupt
#define UIER_RX_LINE_STAT  (1 << 2)    //(UIER_ELSI) Enable Receive Line Status Interrupt
#define UIER_MODEM_STAT_INT_EN (1 << 3)  //(UIER_EDSSI)
#define UIER_CTS_INT_ENT   (1 << 7)
#define UIER_ABTOIntEn     (1 << 8)
#define UIER_ABE0IntEn     (1 << 9)

//Interrupt ID Register bit definitions
#define UIIR_NO_INT         (1 << 0)    //NO INTERRUPTS PENDING if set
#define UIIR_MS_INT         (0 << 1)    //MODEM Status
#define UIIR_THRE_INT       (1 << 1)    //Transmit Holding Register Empty
#define UIIR_RDA_INT        (2 << 1)    //Receive Data Available
#define UIIR_RLS_INT        (3 << 1)    //Receive Line Status
#define UIIR_CTI_INT        (6 << 1)    //Character Timeout Indicator
#define UIIR_ID_MASK        0x0E
#define UIIR_FIFO_ENABLE    (1 << 6)
#define UIIR_ABEOInt        (1 << 8)
#define UIIR_ABTOInt        (1 << 9)

//FIFO Control Register bit definitions
#define UFCR_FIFO_ENABLE    (1 << 0)    //FIFO Enable
#define UFCR_RX_FIFO_RESET  (1 << 1)    //Reset Receive FIFO
#define UFCR_TX_FIFO_RESET  (1 << 2)    //Reset Transmit FIFO
#define UFCR_FIFO_TRIG1     (0 << 6)    //Trigger @ 1 character in FIFO
#define UFCR_FIFO_TRIG4     (1 << 6)    //Trigger @ 4 characters in FIFO
#define UFCR_FIFO_TRIG8     (2 << 6)    //Trigger @ 8 characters in FIFO
#define UFCR_FIFO_TRIG14    (3 << 6)    //Trigger @ 14 characters in FIFO

//Line Control Register bit definitions
#define ULCR_CHAR_5         (0 << 0)    //5-bit character length
#define ULCR_CHAR_6         (1 << 0)    //6-bit character length
#define ULCR_CHAR_7         (2 << 0)    //7-bit character length
#define ULCR_CHAR_8         (3 << 0)    //8-bit character length
#define ULCR_STOP_1         (0 << 2)    //1 stop bit
#define ULCR_STOP_2         (1 << 2)    //2 stop bits
#define ULCR_PAR_NO         (0 << 3)    //No Parity
#define ULCR_PAR_ODD        (1 << 3)    //Odd Parity
#define ULCR_PAR_EVEN       (3 << 3)    //Even Parity
#define ULCR_PAR_MARK       (5 << 3)    //MARK "1" Parity
#define ULCR_PAR_SPACE      (7 << 3)    //SPACE "0" Paruty
#define ULCR_BREAK_ENABLE   (1 << 6)    //Output BREAK line condition
#define ULCR_DLAB_ENABLE    (1 << 7)    //Enable Divisor Latch Access

//Modem Control Register bit definitions
#define UMCR_DTR            (1 << 0)    //Data TerMinal Ready
#define UMCR_RTS            (1 << 1)    //Request To Send
#define UMCR_LB             (1 << 4)    //Loopback

//Line Status Register bit definitions
#define ULSR_RDR            (1 << 0)    //Receive Data Ready
#define ULSR_OE             (1 << 1)    //Overrun Error
#define ULSR_PE             (1 << 2)    //Parity Error
#define ULSR_FE             (1 << 3)    //FraMing Error
#define ULSR_BI             (1 << 4)    //Break Interrupt
#define ULSR_THRE           (1 << 5)    //Transmit Holding Register Empty
#define ULSR_TEMT           (1 << 6)    //Transmitter Empty
#define ULSR_RXFE           (1 << 7)    //Error in Receive FIFO
#define ULSR_ERR_MASK       0x1E

//Modem Status Register bit definitions
#define UMSR_DCTS           (1 << 0)    //Delta Clear To Send
#define UMSR_DDSR           (1 << 1)    //Delta Data Set Ready
#define UMSR_TERI           (1 << 2)    //Trailing Edge Ring Indicator
#define UMSR_DDCD           (1 << 3)    //Delta Data Carrier Detect
#define UMSR_CTS            (1 << 4)    //Clear To Send
#define UMSR_DSR            (1 << 5)    //Data Set Ready
#define UMSR_RI             (1 << 6)    //Ring Indicator
#define UMSR_DCD            (1 << 7)    //Data Carrier Detect

///////////////////////////////////////////////////////////////////////////////
//TIMER defines

//Timer Interrupt Register Bit Definitions
#define TIR_MR0I    (1 << 0)            //Interrupt flag for match channel 0
#define TIR_MR1I    (1 << 1)            //Interrupt flag for match channel 1
#define TIR_MR2I    (1 << 2)            //Interrupt flag for match channel 2
#define TIR_MR3I    (1 << 3)            //Interrupt flag for match channel 3
#define TIR_CR0I    (1 << 4)            //Interrupt flag for capture channel 0 event
#define TIR_CR1I    (1 << 5)            //Interrupt flag for capture channel 1 event
#define TIR_CR2I    (1 << 6)            //Interrupt flag for capture channel 2 event
#define TIR_CR3I    (1 << 7)            //Interrupt flag for capture channel 3 event

//PWM Interrupt Register Bit Definitions
#define PWMIR_MR0I  (1 << 0)            //Interrupt flag for match channel 0
#define PWMIR_MR1I  (1 << 1)            //Interrupt flag for match channel 1
#define PWMIR_MR2I  (1 << 2)            //Interrupt flag for match channel 2
#define PWMIR_MR3I  (1 << 3)            //Interrupt flag for match channel 3
#define PWMIR_MR4I  (1 << 8)            //Interrupt flag for match channel 4
#define PWMIR_MR5I  (1 << 9)            //Interrupt flag for match channel 5
#define PWMIR_MR6I  (1 << 10)           //Interrupt flag for match channel 6
#define PWMIR_MASK  (0x070F)

//Timer Control Register Bit Definitions
#define TCR_ENABLE  (1 << 0)
#define TCR_RESET   (1 << 1)

//PWM Control Register Bit Definitions
#define PWMCR_ENABLE (1 << 0)
#define PWMCR_RESET (1 << 1)

//Timer Match Control Register Bit Definitions
#define TMCR_MR0_I  (1 << 0)            //Enable Interrupt when MR0 matches TC
#define TMCR_MR0_R  (1 << 1)            //Enable Reset of TC upon MR0 match
#define TMCR_MR0_S  (1 << 2)            //Enable Stop of TC upon MR0 match
#define TMCR_MR1_I  (1 << 3)            //Enable Interrupt when MR1 matches TC
#define TMCR_MR1_R  (1 << 4)            //Enable Reset of TC upon MR1 match
#define TMCR_MR1_S  (1 << 5)            //Enable Stop of TC upon MR1 match
#define TMCR_MR2_I  (1 << 6)            //Enable Interrupt when MR2 matches TC
#define TMCR_MR2_R  (1 << 7)            //Enable Reset of TC upon MR2 match
#define TMCR_MR2_S  (1 << 8)            //Enable Stop of TC upon MR2 match
#define TMCR_MR3_I  (1 << 9)            //Enable Interrupt when MR3 matches TC
#define TMCR_MR3_R  (1 << 10)           //Enable Reset of TC upon MR3 match
#define TMCR_MR3_S  (1 << 11)           //Enable Stop of TC upon MR3 match

//Timer Capture Control Register Bit Definitions
#define TCCR_CR0_R (1 << 0)            //Enable Rising edge on CAPn.0 will load TC to CR0
#define TCCR_CR0_F (1 << 1)            //Enable Falling edge on CAPn.0 will load TC to CR0
#define TCCR_CR0_I (1 << 2)            //Enable Interrupt on load of CR0
#define TCCR_CR1_R (1 << 3)            //Enable Rising edge on CAPn.1 will load TC to CR1
#define TCCR_CR1_F (1 << 4)            //Enable Falling edge on CAPn.1 will load TC to CR1
#define TCCR_CR1_I (1 << 5)            //Enable Interrupt on load of CR1
#define TCCR_CR2_R (1 << 6)            //Enable Rising edge on CAPn.2 will load TC to CR2
#define TCCR_CR2_F (1 << 7)            //Enable Falling edge on CAPn.2 will load TC to CR2
#define TCCR_CR2_I (1 << 8)            //Enable Interrupt on load of CR2
#define TCCR_CR3_R (1 << 9)            //Enable Rising edge on CAPn.3 will load TC to CR3
#define TCCR_CR3_F (1 << 10)           //Enable Falling edge on CAPn.3 will load TC to CR3
#define TCCR_CR3_I (1 << 11)           //Enable Interrupt on load of CR3

#define VECT_ADDR_INDEX	0x100
#define VECT_CNTL_INDEX 0x200
#define VECT_PRIO_INDEX 0x200


//SPI defines

#define SPI0_PINSEL_SCK  PINSEL3
#define SPI0_PINSEL_M  PINSEL3
#define SPI0_MISO_LOW  (1 << 14)
#define SPI0_MISO_HIGH (1 << 15)
#define SPI0_MOSI_LOW  (1 << 16)
#define SPI0_MOSI_HIGH (1 << 17)
#define SPI0_SCK0_LOW  (1 << 8)
#define SPI0_SCK0_HIGH ( 1<< 9)

/* SPI0 (Serial Peripheral Interface 0) */
#define SPI0_BASE_ADDR		0xE0020000
#define S0SPCR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x00))
#define S0SPSR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x04))
#define S0SPDR         (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x08))
#define S0SPCCR        (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x0C))
#define S0SPINT        (*(volatile unsigned long *)(SPI0_BASE_ADDR + 0x1C))

/* SSP0 Controller */
#define SSP0_BASE_ADDR		0xE0068000
#define SSP0CR0        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x00))
#define SSP0CR1        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x04))
#define SSP0DR         (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x08))
#define SSP0SR         (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x0C))
#define SSP0CPSR       (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x10))
#define SSP0IMSC       (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x14))
#define SSP0RIS        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x18))
#define SSP0MIS        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x1C))
#define SSP0ICR        (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x20))
#define SSP0DMACR      (*(volatile unsigned long *)(SSP0_BASE_ADDR + 0x24))

/* SSP1 Controller */
#define SPI1_PINSEL_REG  PINSEL0
#define SPI1_MISO_LOW  (0 << 16)
#define SPI1_MISO_HIGH (1 << 17)
#define SPI1_MOSI_LOW  (0 << 18)
#define SPI1_MOSI_HIGH (1 << 19)
#define SSP1_BASE_ADDR		0xE0030000
#define SSP1CR0        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x00))
#define SSP1CR1        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x04))
#define SSP1DR         (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x08))
#define SSP1SR         (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x0C))
#define SSP1CPSR       (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x10))
#define SSP1IMSC       (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x14))
#define SSP1RIS        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x18))
#define SSP1MIS        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x1C))
#define SSP1ICR        (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x20))
#define SSP1DMACR      (*(volatile unsigned long *)(SSP1_BASE_ADDR + 0x24))







#endif

