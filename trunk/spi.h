/*
	file    spi.h
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


#ifndef Spid_H
#define Spid_H



/* this flag is to test SPI in either interrupt mode or polling */
#define INTERRUPT_MODE	1

/* Delay count after each write */
#define DELAY_COUNT		10

#define SPI0_INT_FLAG	0x01

#define MaX_TIMEOUT		0xFF

#define SPI0_ABORT		0x01		/* below two are SPI0 interrupt */
#define SPI0_MODE_FAULT	0x02
#define SPI0_OVERRUN	0x04
#define SPI0_COL		0x08
#define SPI0_TX_DONE	0x10

#define ABRT		BIT(3)		/* SPI0 interrupt status */
#define MODF		BIT(4)
#define ROVR		BIT(5)
#define WCOL		BIT(6)
#define SPIF		BIT(7)

#define RORIC		0x00000001
#define RTIC		0x00000002

/* SPI 0 PCR register */
#define SPI0_BE		0x00000004
#define SPI0_CPHA	0x00000008
#define SPI0_CPOL	0x00000010
#define SPI0_MSTR	0x00000020
#define SPI0_LSBF	0x00000040
#define SPI0_SPIE	0x00000080

volatile unsigned long TxCounter;
volatile int SPI0Status;

/* SPI read and write buffer size */
#define BUFSIZE			256
#define FIFOSIZE		8				//toDo check if this is really 8 bytes fifo

/* SSP Status register */
#define SSPSR_TFE		1 << 0
#define SSPSR_TNF		1 << 1
#define SSPSR_RNE		1 << 2
#define SSPSR_RFF		1 << 3
#define SSPSR_BSY		1 << 4

/* SSP0 CR0 register */
#define SSPCR0_DSS		1 << 0
#define SSPCR0_FRF		1 << 4
#define SSPCR0_SPO		1 << 6
#define SSPCR0_SPH		1 << 7
#define SSPCR0_SCR		1 << 8

/* SSP0 CR1 register */
#define SSPCR1_LBM		1 << 0
#define SSPCR1_SSE		1 << 1
#define SSPCR1_MS		1 << 2
#define SSPCR1_SOD		1 << 3

/* SSP0 Interrupt Mask Set/Clear register */
#define SSPIMSC_RORIM	1 << 0
#define SSPIMSC_RTIM	1 << 1
#define SSPIMSC_RXIM	1 << 2
#define SSPIMSC_TXIM	1 << 3

/* SSP0 Interrupt Status register */
#define SSPRIS_RORRIS	1 << 0
#define SSPRIS_RTRIS	1 << 1
#define SSPRIS_RXRIS	1 << 2
#define SSPRIS_TXRIS	1 << 3

/* SSP0 Masked Interrupt register */
#define SSPMIS_RORMIS	1 << 0
#define SSPMIS_RTMIS	1 << 1
#define SSPMIS_RXMIS	1 << 2
#define SSPMIS_TXMIS	1 << 3

/* SSP0 Interrupt clear register */
#define SSPICR_RORIC	1 << 0
#define SSPICR_RTIC		1 << 1

void spi0_isr (void);
void initSSPI0IRQ(void);

void SSPI1Init(void);
void SPISend( char *Buf, unsigned long Length );
void SPIReceive( char *Buf, unsigned long Length );
char SPIReceiveByte( void );

#endif  /* __Spid_H__ */
/*****************************************************************************
**                            End Of File
******************************************************************************/

