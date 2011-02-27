/*
	file    init.c
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

#include "arch/settings.h"
#include "arch/init.h"
#include "arch/clock-arch.h"
#include "arch/sys_config.h"
#include "arch/analog.h"
#include "arch/led.h"
#include "arch/viclowlevel.h"
#include "arch/beeper.h"

#include "io/io.h"
#include "io/spi.h"
#include "io/pwmout.h"
#include "io/pwmin.h"
#include "io/uartisr.h"
#include "io/uart.h"
#include "io/i2c.h"
#include "io/engines.h"
#include "io/compass.h"

#include "fc/osifc.h"
#include "nav/mm3parser.h"

#include "interface/command.h"

static int RTCinit = 0;


/* Initialize the interrupt controller */

void init_VIC(void)
{
    unsigned long i = 0;
    unsigned long *vect_addr, *vect_cntl;

    /* initialize VIC*/
    VICIntEnClr = 0xFFFFFFFF;
    VICVectAddr = 0;
    VICIntSelect = 0;

    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
		vect_addr = (unsigned long *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
		vect_cntl = (unsigned long *)(VIC_BASE_ADDR + VECT_CNTL_INDEX + i*4);
		*vect_addr = 0x0;
		*vect_cntl = 0xF;
    }
	VICIntEnable = 0x00;	/* disable all Interrupts */

    return;
}


void reset_GPIO( void )
{
	/* Reset all GPIO pins to default: primary function */
    PINSEL0 = 0x00000000;
    PINSEL1 = 0x00000000;
    PINSEL2 = 0x00000000;
    PINSEL3 = 0x00000000;
    PINSEL4 = 0x00000000;
    PINSEL5 = 0x00000000;
    PINSEL6 = 0x00000000;
    PINSEL7 = 0x00000000;
    PINSEL8 = 0x00000000;
    PINSEL9 = 0x00000000;
    PINSEL10 = 0x00000000;

    SCS |= (1<<0); //set GPIOM for P0 and P1 to be FGPIO

    FIO0DIR = 0x00000000;
    FIO1DIR = 0x00000000;
    FIO2DIR = 0x00000000;
    FIO3DIR = 0x00000000;
    FIO4DIR = 0x00000000;

    FIO0SET = 0x00000000;
    FIO1SET = 0x00000000;
    FIO2SET = 0x00000000;
    FIO3SET = 0x00000000;
    FIO4SET = 0x00000000;
}


void init_PLL(void)
{
	unsigned long readback;

		//check if PLL connected, disconnect if yes
		if ( PLLSTAT & PLLSTAT_PLLC ) {
			PLLCON = PLLCON_PLLE;       /* Enable PLL, disconnected ( PLLC = 0 )*/
			PLLFEED = 0xaa;
			PLLFEED = 0x55;
		}

		PLLCON  = 0;        /* Disable PLL, disconnected */
		PLLFEED = 0xaa;
		PLLFEED = 0x55;

		SCS |= SCS_OSCEN;   /* Enable main OSC, SCS Man p.28 */
		while( !( SCS & SCS_OSCSTAT ) ) {
			;	/* Wait until main OSC is usable */
		}

		CLKSRCSEL = CLKSRC_MAIN_OSC;   /* select main OSC as the PLL clock source */

		PLLCFG = PLLCFG_MSEL | PLLCFG_NSEL;
		PLLFEED = 0xaa;
		PLLFEED = 0x55;

		PLLCON = PLLCON_PLLE;       /* Enable PLL, disconnected ( PLLC = 0 ) */
		PLLFEED = 0xaa;
		PLLFEED = 0x55;

		CCLKCFG = CCLKCFG_CCLKSEL_VAL;     /* Set clock divider, Manual p.45 */


		while ( ( PLLSTAT & PLLSTAT_PLOCK ) == 0 )  {
			; /* Check lock bit status */
		}

		readback = PLLSTAT & 0x00FF7FFF;
		while ( readback != (PLLCFG_MSEL | PLLCFG_NSEL) )
		{
			; //stall - invalid readback PCLKSEL0
		}

		PLLCON = ( PLLCON_PLLE | PLLCON_PLLC );  /* enable and connect */
		PLLFEED = 0xaa;
		PLLFEED = 0x55;
		while ( ((PLLSTAT & PLLSTAT_PLLC) == 0) ) {
			;  /* Check connect bit status, wait for connect */
		}
}
//here we power on and of the devices within the LPC
//this means we actually give current to the peace of hardware or connect it to the clock
//this is mainly made for power saving so if you like to save power think about saving it here
//Don't switch off what you need ;)

void init_Powr(void)
{
	//set ADC to full stop
	AD0CR =  (0 << 21); 			//has to be set to 0 before switching ADC power


									//PCONP 0 is unused and should not been set
	PCONP |= (1 << 1);  			//Timer/Counter 0 power/clock control bit.
	PCONP |= (1 << 2);				//Timer/Counter 1 power/clock control bit.
	PCONP |= (1 << 3);				//UART0
	PCONP |= (0 << 4);				//UART1 not yet used so switched off

	//Bit 5 is unused by LPC2378

	PCONP |= (1 << 6);  			//PWM1 power/clock control bit.
									//TODO PWM1 to been switched on once start sending PWM out to the PWM channels
									//in any way this is NOT needed to READ PWM

	PCONP |= (1 << 7);				//The I2C0 interface power/clock control bit.
	PCONP |= (0 << 8);				//The SPI interface power/clock control bit. not used we use SSP0 in SPI mode instead
	PCONP |= (1 << 9); 				//The RTC power/clock control bit.
	PCONP |= (0 << 10);				//The SSP1 interface power/clock control bit. (SPI1)
	PCONP |= (0 << 11);				//External Memory Controller not build in
	PCONP |= (1 << 12);				//A/D converter (ADC) power/clock control bit.
	PCONP |= (0 << 13);				//CAN Controller 1 power/clock control bit not build in
	PCONP |= (0 << 14);				//CAN Controller 2 power/clock control bit	not build in
	//Bits 15-18 are unused by LPC2378
	PCONP |= (1 << 19); 			//The I2C1 interface power/clock control bit

	PCONP |= (1 << 21); 			//The SSP0 interface power/clock control bit.

	PCONP |= (0 << 22); 			//Timer 2 power/clock control bit.
	PCONP |= (0 << 23); 			//Timer 3 power/clock control bit.
									//switch on timers when using them
	PCONP |= (1 << 24); 			//UART2 used to retrieve GPS Data
									//since this is full duplex you still have TXD free on UART2
	PCONP |= (1 << 25); 			//UART3 used for BT


	PCONP |= (0 << 26); 			//I2C interface 2 power/clock control bit.
									//can be used later
	PCONP |= (0 << 27);				//I2S interface power/clock control bit.
	PCONP |= (0 << 28);				//SD card interface power/clock control bit.
									//can be used later
	PCONP |= (0 << 29);				//GP DMA function power/clock control bit. not build in osiFC
	PCONP |= (0 << 30);				//Ethernet block power/clock control bit. not build in osiFC
	PCONP |= (0 << 31);				//USB interface power/clock control bit. not build in osiFC

	PINSEL10 |= (0 << 3);			// disable ETM interface


	//note  13, 14, 21, 29, 30, 31 bits should not been set to 1 as those functions are
	//mainly not build into this board and may cause damage or faulty results
	//all the rest may become useful in the future
}




void init_LowLevelSystems(void)
{
	init_Powr();
	init_PLL();
	//setup & enable the MAM
	MAMCR = MAMCR_OFF;
#if CCLK < 20000000
	MAMTIM = 1;
#elif CCLK < 40000000
	MAMTIM = 2;
#else
	MAMTIM = 3;
#endif
	MAMCR = MAMCR_MODE;

	//set the peripheral bus speed
	//value computed from config.h

#if PBSD == 4
	PCLKSEL0 = 0x01195014;	/* PCLK is 1/4 CCLK */ //1000110010101000000010100
	PCLKSEL1 = 0x11105454;  //10001000100000101010001010100
#elif PBSD == 2
	PCLKSEL0 = 0xAAAAAAAA;	/* PCLK is 1/2 CCLK */
	PCLKSEL1 = 0xAAAAAAAA;
#elif PBSD == 1
	PCLKSEL0 = 0x55515155;	/* PCLK is the same as CCLK */
	PCLKSEL1 = 0x11555455;
#else
#error invalid p-clock divider
#endif
/*	//set the interrupt controller defaults
#if defined(RAM_RUN)
	MEMMAP = MEMMAP_SRAM;                 //map interrupt vectors space into Static RAM
#elif defined(ROM_RUN)
	MEMMAP = MEMMAP_FLASH;                //map interrupt vectors space into FLASH
#else
#error RUN_MODE not defined!
#endif */
}

void init_ADC_HW(void)
{

	ADC_OFF;

	/* all the related pins are set to ADC inputs, AD0.0~7 */
	PINSEL0 |= 0x0F000000;  /* P0.12~13, A0.6~7, function 11 */
	PINSEL1 &= ~0x003FC000; /* P0.23~26, A0.0~3, function 01 */
	PINSEL1 |= 0x00154000;
	PINSEL3 |= 0xF0000000;  /* P1.30~31, A0.4~5, function 11 */


	AD0CR = ( 0 << 21 );

	ADC_CLK_DIV = sysSetup.CYCLE.AdcClockDiv;

	/* SEL=1,select channel 0~7 on ADC0 */
	AD0CR = ( 1 << 0 ) |   		//channel
			( 1 << 1 ) |		//channel
  		  	( 1 << 2 ) |	 	//channel
  		  	( 1 << 3 ) |	 	//channel
  		  	( 1 << 4 ) |	 	//channel
  		  	( 1 << 5 ) |	 	//channel
  		  	( 1 << 6 ) |	 	//channel
  		  	( 1 << 7 ) | 	 	//channel

  		  	( (ADC_CLK_DIV) << 8 ) |
  		  	( 1 << 16 ) |     /* BURST = 0 -> no BURST, software controlled */
  		  	( 0 << 17 ) |     /* CLKS = 0, 11 clocks/10 bits */
  		  	( 0 << 18 ) |
  		  	( 0 << 19 ) |
  		  	( 1 << 21 ) |     /* PDN = 1, normal operation */
  		  	( 0 << 22 ) |     /* TEST1:0 = 00 */
  		  	( 0 << 24 ) |     /* START = 0 A/D conversion stops */
  		  	( 0 << 27 );      /* EDGE = 0 (CAP/MAT signal falling,trigger A/D conversion) */


  	resetADCmuxer();
}

void init_LED(void)
{


	LED1_DIR |= LED1_BIT; //LED-Pin as output
	LED2_DIR |= LED2_BIT; //LED-Pin as output
	LED3_DIR |= LED3_BIT; //LED-Pin as output
	LED4_DIR |= LED4_BIT; //LED-Pin as output

	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;

}

void init_PWMinChannels(void)
{
	//this may look like it does make no sense it does not! but just to be sure...
	PINSEL0 |= (0<<8)	|(0<<9);
	PINSEL0 |= (0<<10)	|(0<<11);
	PINSEL0 |= (0<<12)	|(0<<13);
	PINSEL0 |= (0<<30)	|(0<<31);
	PINSEL1 |= (0<<0)	|(0<<1);
	PINSEL1 |= (0<<2)	|(0<<3);
	PINSEL1 |= (0<<4)	|(0<<5);
	PINSEL4 |= (0<<10)	|(0<<11);
	PINSEL4 |= (0<<14)	|(0<<15);

}

void init_sspi0(void)
{
	char i, tmp;
	//set SPI0 Pins
	SPI0_PINSEL_SCK |= SPI0_SCK0_LOW | SPI0_SCK0_HIGH;
	SPI0_PINSEL_M |= SPI0_MISO_LOW | SPI0_MISO_HIGH;
	SPI0_PINSEL_M |= SPI0_MOSI_LOW | SPI0_MOSI_HIGH;

	PINMODE3 |= 0x00003000;

	TxCounter = 0;
	SPI0Status = 0;

	PCLKSEL1  	|= BIT(11) | NBIT(10);
	SSP0CR0 	|= 0x5007; 				//set to 8 Bit(0111),FRF = 0, CPOL = 0, CPHA = 0, SCR = 0x50
	SSP0CR0 	|= NBIT(4) | NBIT(5); 	//SPI Mode
	SSP0CPSR 	= 0x32; 				//prescaler should be Minimum 2 as bit 0 is always 0 by hardware
	SSP0IMSC 	|= BIT(0) | BIT(1);		//enable interrupt for any error cause this is mainly to clear te bits to keep the bus working
	SSP0CR1 	|= BIT(1); 				//enable SSP0
	initSSPI0IRQ();

	for ( i = 0; i < FIFOSIZE; i++ )
	{
		tmp = SSP0DR;		/* clear the RxFIFO */
	}

}
void init_spi1(void)
{

	//set SPI1 Pins
	SPI1_PINSEL_REG |= (0<<14)|(1<<15);
	SPI1_PINSEL_REG |= SPI1_MISO_LOW | SPI1_MISO_HIGH;
	SPI1_PINSEL_REG |= SPI1_MOSI_LOW | SPI1_MOSI_HIGH;
}


void init_SDcard(void)
{
	PINSEL1 |= (1<<6)|(0<<7);
	PINSEL1 |= (1<<8)|(0<<9);
	PINSEL1 |= (1<<10)|(0<<11);
	PINSEL1 |= (1<<12)|(0<<13);
	PINSEL4 |= (1<<22)|(0<<23);
	PINSEL4 |= (1<<24)|(0<<25);
	PINSEL4 |= (1<<26)|(0<<26);
	//WP CP set to GPIO
	PINSEL1 |= (0<<26)|(0<<27);
	PINSEL1 |= (0<<28)|(0<<29);
}
void init_PWM(void)
{

	switch (fcSetup.PWMMode) //fcSetup.escType
    {
	case PWM_MC:
		//Interrupt bei rising edge
		//P2.6 to Capture
		//P2.2 to Capture MM3 DRDY
		IO2_INT_EN_R |= pwmin0_BIT | DRDY_PIN;
		VICVectAddr17     = (unsigned long)pwmin_MC_ISR;
	break;
	case PWM_SC:
		//Capture
		//pins 0.4, 0.5, 0.6, 0.15, 0.16, 0.17, 0.18, 2.5, 2.7 (E0)
		//2.2 gets used as DRDY (04) Rising edge only
		//if you like or need you can add the pin used for PWMtmp signal
		//this would the allow you to have 10 PWM in channels
		//Interrupt at rising edge
		IO0_INT_EN_R |= 0x00078070;
		IO2_INT_EN_R |= 0x000000A4;
		VICVectAddr17     = (unsigned long)pwmin_SC_ISR;
	break;
    }


	//VICIntSelect
	//initialize the interrupt vector
	VICIntSelect    &= ~VIC_CHAN_TO_MASK(VIC_CHAN_NUM_EINT3);
	VICIntEnClr     = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_EINT3);
	VICVectPriority17 = 0x02;
	VICIntEnable    = VIC_CHAN_TO_MASK(VIC_CHAN_NUM_EINT3);
}
void init_uart(void)
{
	//set port pins for UART0

	U0_TX_PINSEL_REG |= ( U0_TX_PINSEL_REG & ~U0_TX_PINMASK ) | U0_TX_PINSEL;
	U0_RX_PINSEL_REG |= ( U0_RX_PINSEL_REG & ~U0_RX_PINMASK ) | U0_RX_PINSEL;

	//U0_TX_PINSEL_REG |= U0_TX_LOW;
	//U0_TX_PINSEL_REG |= U0_TX_HIGH;
	//U0_RX_PINSEL_REG |= U0_RX_LOW;
	//U0_RX_PINSEL_REG |= U0_RX_HIGH;

	//set port pins for UART1
	//U1_TX_PINSEL_REG |= U1_TX_LOW;
	//U1_TX_PINSEL_REG |= U1_TX_HIGH;
	//U1_RX_PINSEL_REG |= U1_RX_LOW;
	//U1_RX_PINSEL_REG |= U1_RX_HIGH;

	//set port pins for UART2
	U2_TX_PINSEL_REG |= U2_TX_LOW;
	U2_TX_PINSEL_REG |= U2_TX_HIGH;
	U2_RX_PINSEL_REG |= U2_RX_LOW;
	U2_RX_PINSEL_REG |= U2_RX_HIGH;

	//set port pins for UART3
	U3_TX_PINSEL_REG |= U3_TX_LOW;
	U3_TX_PINSEL_REG |= U3_TX_HIGH;
	U3_RX_PINSEL_REG |= U3_RX_LOW;
	U3_RX_PINSEL_REG |= U3_RX_HIGH;


	uart0Init(B115200, UART_8N1, UART_FIFO_8); //setup the UART0			//main UART supports direct PC connection
	//uart1Init(B115200, UART_8N1, UART_FIFO_8); //setup the UART1			//not in use yet
	uart2Init(B115200, UART_8N1, UART_FIFO_8); //setup the UART2	  		//UART used for GPS
	uart3Init(B115200, UART_8N1, UART_FIFO_8); //setup the UART3			//UART connected to bluetooth but not yet in use
}
void init_RTC(void)
{
	RTC_CTCReset();
	RTCInit();
	RTCStart();
}

void init_MM3(void)
{
	//IO1(RESET) and IO0 (SSNOT) as out
	ioInit0();			//SlaveSelect
	ioInit1();			//Reset

	IO0_ON;				//IO0 (SSNOT) auf High -> MM3 passive
	IO1_OFF;			//IO1 (RESET) auf Low

	//Init Statemachine
	MM3_runtime.AXIS = MM3_X;
	MM3_runtime.STATE = MM3_RESET;
	//led_switch(2);
}



void init_system(void)
{
	//here we init the whole hardware park
	init_LowLevelSystems();                	//setup clocks and processor port pins
	reset_GPIO();					  		//setup all ports to be IO
	init_beeper();
	init_VIC();							  	//init the Vector Interrupt
	enableIRQ(); 						  	//switch on the IRQ

	init_uart();							//init the UART
	print_uart0("FCm0;low level system alive;00#");

	init_LED();							  	//init the LEDs

	ledOFF();
	ledTest();							 	//test the LEDs

	print_uart0("FCm0;starting secondary systems;00#");

	enginesOff();						  	//set engines to 0 PWM

	initSettings();						  	//load the settings from flash to ram

	ygeStartI2CCounter = 0;
	engine = 0;

	ADC_offset[0] = 125;
	ADC_offset[1] = 125;
	ADC_offset[2] = 125;
	ADC_offset[6] = 200;




	I2C0Init();						  	 	//init I2C0 Port (Engines)
	I2C0Start();

	I2C1Init();								//init I2C1 Port DAC
	I2C1Start();

	//I2C2Init();						 	//not yet in use
	//I2C2Start();



	if ( RTCinit == 0) {
		RTCinit = 1;
		init_RTC();						  	//init the RTC which has no buffer so there is no real trustable date
											//but its a good source for time, counting seconds, Minutes and hours...
	}

	//init_timer0();					  	//start Timer0 not in use yet
	init_timer1();					  	  	//timer 1 gets used by PWM
	//init_timer2();					  	//start Timer2 not in use yet
	//init_timer3();					  	//start Timer3 not in use yet

	init_sspi0();						  	//init sspi0 using spi



	initPWMvars();
	init_PWM();
	initCamServos();
	init_PWMOutChannels();					//init the PWM out Pins



	engine = 0;

	print_uart0("FCm0;setting up flight system;00#");
	initFCRuntime();
	init_ADC_HW();
	print_uart0("FCm0;capture ADC standstill values;00#");

	resetADCmuxer();
	GyroInit();
	//ADCPressureSetup(AIRPRESSURE);
	ADCStandstillValues();
	init_MM3();							 	//init the MM3
											//this also inits IO0 and IO1 which are taken for CS(SS) and reset

	//print_uart0("FCm0;init HMC;00#");
	//I2C1InitHMCMode();

	//setHMC5843Gain();
	//initHMC6343();

	//setHMC5843Rate();

	//setHMCGain();

	//setHMC5843Continous();


	//for(int tmp = 0;tmp<1000000;tmp++){asm("nop");};
	//print_uart0("FCm0;read HMC;00#");
	//initialreadHMC5843();

	I2C1Stop();
	LED1_ON;


}




