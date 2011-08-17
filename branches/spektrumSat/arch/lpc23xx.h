/******************************************************************************
 *   LPC23xx.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *   The header file is the super set of all hardware definition of the
 *   peripherals for the LPC23xx/24xx family microprocessor.
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
 ******************************************************************************/

/* Modified by Martin Thomas */

#ifndef __LPC23xX_H
#define __LPC23xX_H

/* Vectored Interrupt Controller (VIC) */
#define VIC_BASE_ADDR	0xFFFFF000
#define VICIRQStatus   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x000))
#define VICFIQStatus   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x004))
#define VICRawIntr     (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x008))
#define VICIntSelect   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x00C))
#define VICIntEnable   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x010))
#define VICIntEnClr    (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x014))
#define VICSoftInt     (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x018))
#define VICSoftIntClr  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x01C))
#define VICProtection  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x020))
#define VICSWPrioMask  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x024))

#define VICVectAddr0   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x100))
#define VICVectAddr1   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x104))
#define VICVectAddr2   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x108))
#define VICVectAddr3   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x10C))
#define VICVectAddr4   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x110))
#define VICVectAddr5   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x114))
#define VICVectAddr6   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x118))
#define VICVectAddr7   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x11C))
#define VICVectAddr8   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x120))
#define VICVectAddr9   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x124))
#define VICVectAddr10  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x128))
#define VICVectAddr11  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x12C))
#define VICVectAddr12  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x130))
#define VICVectAddr13  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x134))
#define VICVectAddr14  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x138))
#define VICVectAddr15  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x13C))
#define VICVectAddr16  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x140))
#define VICVectAddr17  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x144))
#define VICVectAddr18  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x148))
#define VICVectAddr19  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x14C))
#define VICVectAddr20  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x150))
#define VICVectAddr21  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x154))
#define VICVectAddr22  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x158))
#define VICVectAddr23  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x15C))
#define VICVectAddr24  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x160))
#define VICVectAddr25  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x164))
#define VICVectAddr26  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x168))
#define VICVectAddr27  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x16C))
#define VICVectAddr28  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x170))
#define VICVectAddr29  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x174))
#define VICVectAddr30  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x178))
#define VICVectAddr31  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x17C))


#define VICVectPriority0   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x200))
#define VICVectPriority1   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x204))
#define VICVectPriority2   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x208))
#define VICVectPriority3   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x20C))
#define VICVectPriority4   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x210))
#define VICVectPriority5   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x214))
#define VICVectPriority6   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x218))
#define VICVectPriority7   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x21C))
#define VICVectPriority8   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x220))
#define VICVectPriority9   (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x224))
#define VICVectPriority10  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x228))
#define VICVectPriority11  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x22C))
#define VICVectPriority12  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x230))
#define VICVectPriority13  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x234))
#define VICVectPriority14  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x238))
#define VICVectPriority15  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x23C))
#define VICVectPriority16  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x240))
#define VICVectPriority17  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x244))
#define VICVectPriority18  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x248))
#define VICVectPriority19  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x24C))
#define VICVectPriority20  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x250))
#define VICVectPriority21  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x254))
#define VICVectPriority22  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x258))
#define VICVectPriority23  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x25C))
#define VICVectPriority24  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x260))
#define VICVectPriority25  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x264))
#define VICVectPriority26  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x268))
#define VICVectPriority27  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x26C))
#define VICVectPriority28  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x270))
#define VICVectPriority29  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x274))
#define VICVectPriority30  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x278))
#define VICVectPriority31  (*(volatile unsigned long *)(VIC_BASE_ADDR + 0x27C))


#define VICVectAddr    (*(volatile unsigned long *)(VIC_BASE_ADDR + 0xF00))


/* Pin Connect Block */
#define PINSEL_BASE_ADDR	0xE002C000
#define PINSEL0        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x00))
#define PINSEL1        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x04))
#define PINSEL2        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x08))
#define PINSEL3        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x0C))
#define PINSEL4        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x10))
#define PINSEL5        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x14))
#define PINSEL6        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x18))
#define PINSEL7        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x1C))
#define PINSEL8        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x20))
#define PINSEL9        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x24))
#define PINSEL10       (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x28))

#define PINMODE0        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x40))
#define PINMODE1        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x44))
#define PINMODE2        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x48))
#define PINMODE3        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x4C))
#define PINMODE4        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x50))
#define PINMODE5        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x54))
#define PINMODE6        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x58))
#define PINMODE7        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x5C))
#define PINMODE8        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x60))
#define PINMODE9        (*(volatile unsigned long *)(PINSEL_BASE_ADDR + 0x64))

/* General Purpose Input/Output (GPIO) */
#define GPIO_BASE_ADDR		0xE0028000
#define IOPIN0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x00))
#define IOSET0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x04))
#define IODIR0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x08))
#define IOCLR0         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x0C))
#define IOPIN1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x10))
#define IOSET1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x14))
#define IODIR1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x18))
#define IOCLR1         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x1C))

/* mthomas - alias */
#define IO0PIN         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x00))
#define IO0SET         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x04))
#define IO0DIR         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x08))
#define IO0CLR         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x0C))
#define IO1PIN         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x10))
#define IO1SET         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x14))
#define IO1DIR         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x18))
#define IO1CLR         (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x1C))


/* GPIO Interrupt Registers */
#define IO0_INT_EN_R    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x90))
#define IO0_INT_EN_F    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x94))
#define IO0_INT_STAT_R  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x84))
#define IO0_INT_STAT_F  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x88))
#define IO0_INT_CLR     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x8C))

#define IO2_INT_EN_R    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xB0))
#define IO2_INT_EN_F    (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xB4))
#define IO2_INT_STAT_R  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xA4))
#define IO2_INT_STAT_F  (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xA8))
#define IO2_INT_CLR     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0xAC))

#define IO_INT_STAT     (*(volatile unsigned long *)(GPIO_BASE_ADDR + 0x80))

#define PARTCFG_BASE_ADDR		0x3FFF8000
#define PARTCFG        (*(volatile unsigned long *)(PARTCFG_BASE_ADDR + 0x00))

/* Fast I/O setup */
#define FIO_BASE_ADDR		0x3FFFC000
#define FIO0DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x00))
#define FIO0MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x10))
#define FIO0PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x14))
#define FIO0SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x18))
#define FIO0CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x1C))

#define FIO1DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x20))
#define FIO1MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x30))
#define FIO1PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x34))
#define FIO1SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x38))
#define FIO1CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x3C))

#define FIO2DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x40))
#define FIO2MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x50))
#define FIO2PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x54))
#define FIO2SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x58))
#define FIO2CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x5C))

#define FIO3DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x60))
#define FIO3MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x70))
#define FIO3PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x74))
#define FIO3SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x78))
#define FIO3CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x7C))

#define FIO4DIR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x80))
#define FIO4MASK       (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x90))
#define FIO4PIN        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x94))
#define FIO4SET        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x98))
#define FIO4CLR        (*(volatile unsigned long *)(FIO_BASE_ADDR + 0x9C))

/* FIOs can be accessed through WORD, HALF-WORD or BYTE. */
#define FIO0DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x00))
#define FIO1DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x20))
#define FIO2DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x40))
#define FIO3DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x60))
#define FIO4DIR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x80))

#define FIO0DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x01))
#define FIO1DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x21))
#define FIO2DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x41))
#define FIO3DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x61))
#define FIO4DIR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x81))

#define FIO0DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x02))
#define FIO1DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x22))
#define FIO2DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x42))
#define FIO3DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x62))
#define FIO4DIR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x82))

#define FIO0DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x03))
#define FIO1DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x23))
#define FIO2DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x43))
#define FIO3DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x63))
#define FIO4DIR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x83))

#define FIO0DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x00))
#define FIO1DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x20))
#define FIO2DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x40))
#define FIO3DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x60))
#define FIO4DIRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x80))

#define FIO0DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x02))
#define FIO1DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x22))
#define FIO2DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x42))
#define FIO3DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x62))
#define FIO4DIRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x82))

#define FIO0MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x10))
#define FIO1MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x30))
#define FIO2MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x50))
#define FIO3MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x70))
#define FIO4MASK0      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x90))

#define FIO0MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x11))
#define FIO1MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x21))
#define FIO2MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x51))
#define FIO3MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x71))
#define FIO4MASK1      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x91))

#define FIO0MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x12))
#define FIO1MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x32))
#define FIO2MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x52))
#define FIO3MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x72))
#define FIO4MASK2      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x92))

#define FIO0MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x13))
#define FIO1MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x33))
#define FIO2MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x53))
#define FIO3MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x73))
#define FIO4MASK3      (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x93))

#define FIO0MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x10))
#define FIO1MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x30))
#define FIO2MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x50))
#define FIO3MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x70))
#define FIO4MASKL      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x90))

#define FIO0MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x12))
#define FIO1MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x32))
#define FIO2MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x52))
#define FIO3MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x72))
#define FIO4MASKU      (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x92))

#define FIO0PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x14))
#define FIO1PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x34))
#define FIO2PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x54))
#define FIO3PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x74))
#define FIO4PIN0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x94))

#define FIO0PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x15))
#define FIO1PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x25))
#define FIO2PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x55))
#define FIO3PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x75))
#define FIO4PIN1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x95))

#define FIO0PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x16))
#define FIO1PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x36))
#define FIO2PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x56))
#define FIO3PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x76))
#define FIO4PIN2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x96))

#define FIO0PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x17))
#define FIO1PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x37))
#define FIO2PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x57))
#define FIO3PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x77))
#define FIO4PIN3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x97))

#define FIO0PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x14))
#define FIO1PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x34))
#define FIO2PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x54))
#define FIO3PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x74))
#define FIO4PINL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x94))

#define FIO0PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x16))
#define FIO1PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x36))
#define FIO2PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x56))
#define FIO3PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x76))
#define FIO4PINU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x96))

#define FIO0SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x18))
#define FIO1SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x38))
#define FIO2SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x58))
#define FIO3SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x78))
#define FIO4SET0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x98))

#define FIO0SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x19))
#define FIO1SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x29))
#define FIO2SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x59))
#define FIO3SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x79))
#define FIO4SET1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x99))

#define FIO0SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1A))
#define FIO1SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3A))
#define FIO2SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5A))
#define FIO3SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7A))
#define FIO4SET2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9A))

#define FIO0SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1B))
#define FIO1SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3B))
#define FIO2SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5B))
#define FIO3SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7B))
#define FIO4SET3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9B))

#define FIO0SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x18))
#define FIO1SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x38))
#define FIO2SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x58))
#define FIO3SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x78))
#define FIO4SETL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x98))

#define FIO0SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1A))
#define FIO1SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3A))
#define FIO2SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5A))
#define FIO3SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7A))
#define FIO4SETU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9A))

#define FIO0CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1C))
#define FIO1CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3C))
#define FIO2CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5C))
#define FIO3CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7C))
#define FIO4CLR0       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9C))

#define FIO0CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1D))
#define FIO1CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x2D))
#define FIO2CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5D))
#define FIO3CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7D))
#define FIO4CLR1       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9D))

#define FIO0CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1E))
#define FIO1CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3E))
#define FIO2CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5E))
#define FIO3CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7E))
#define FIO4CLR2       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9E))

#define FIO0CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x1F))
#define FIO1CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x3F))
#define FIO2CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x5F))
#define FIO3CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x7F))
#define FIO4CLR3       (*(volatile unsigned char *)(FIO_BASE_ADDR + 0x9F))

#define FIO0CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1C))
#define FIO1CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3C))
#define FIO2CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5C))
#define FIO3CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7C))
#define FIO4CLRL       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9C))

#define FIO0CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x1E))
#define FIO1CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x3E))
#define FIO2CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x5E))
#define FIO3CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x7E))
#define FIO4CLRU       (*(volatile unsigned short *)(FIO_BASE_ADDR + 0x9E))


/* System Control Block(SCB) modules include Memory Accelerator Module,
Phase Locked Loop, VPB divider, Power Control, External Interrupt,
Reset, and Code Security/Debugging */
#define SCB_BASE_ADDR	0xE01FC000

/* Memory Accelerator Module (MAM) */
#define MAMCR          (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x000))
#define MAMTIM         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x004))
#define MEMMAP         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x040))

/* Phase Locked Loop (PLL) */
#define PLLCON         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x080))
#define PLLCFG         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x084))
#define PLLSTAT        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x088))
#define PLLFEED        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x08C))

/* Power Control */
#define PCON           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x0C0))
#define PCONP          (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x0C4))

/* Clock Divider */
//#define APBDIV         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x100))
#define CCLKCFG        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x104))
#define USBCLKCFG      (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x108))
#define CLKSRCSEL      (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x10C))
#define PCLKSEL0       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1A8))
#define PCLKSEL1       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1AC))

/* External Interrupts */
#define EXTINT         (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x140))
#define INTWAKE        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x144))
#define EXTMODE        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x148))
#define EXTPOLAR       (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x14C))

/* Reset, reset source identification */
#define RSIR           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x180))

/* RSID, code security protection */
#define CSPR           (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x184))

/* AHB configuration */
#define AHBCFG1        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x188))
#define AHBCFG2        (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x18C))

/* System Controls and Status */
#define SCS            (*(volatile unsigned long *)(SCB_BASE_ADDR + 0x1A0))

/* Timer 0 */
#define TMR0_BASE_ADDR		0xE0004000
#define T0IR           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x00))
#define T0TCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x04))
#define T0TC           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x08))
#define T0PR           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x0C))
#define T0PC           (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x10))
#define T0MCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x14))
#define T0MR0          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x18))
#define T0MR1          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x1C))
#define T0MR2          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x20))
#define T0MR3          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x24))
#define T0CCR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x28))
#define T0CR0          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x2C))
#define T0CR1          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x30))
#define T0CR2          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x34))
#define T0CR3          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x38))
#define T0EMR          (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x3C))
#define T0CTCR         (*(volatile unsigned long *)(TMR0_BASE_ADDR + 0x70))

/* Timer 1 */
#define TMR1_BASE_ADDR		0xE0008000
#define T1IR           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x00))
#define T1TCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x04))
#define T1TC           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x08))
#define T1PR           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x0C))
#define T1PC           (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x10))
#define T1MCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x14))
#define T1MR0          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x18))
#define T1MR1          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x1C))
#define T1MR2          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x20))
#define T1MR3          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x24))
#define T1CCR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x28))
#define T1CR0          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x2C))
#define T1CR1          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x30))
#define T1CR2          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x34))
#define T1CR3          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x38))
#define T1EMR          (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x3C))
#define T1CTCR         (*(volatile unsigned long *)(TMR1_BASE_ADDR + 0x70))

/* Timer 2 */
#define TMR2_BASE_ADDR		0xE0070000
#define T2IR           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x00))
#define T2TCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x04))
#define T2TC           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x08))
#define T2PR           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x0C))
#define T2PC           (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x10))
#define T2MCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x14))
#define T2MR0          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x18))
#define T2MR1          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x1C))
#define T2MR2          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x20))
#define T2MR3          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x24))
#define T2CCR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x28))
#define T2CR0          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x2C))
#define T2CR1          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x30))
#define T2CR2          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x34))
#define T2CR3          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x38))
#define T2EMR          (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x3C))
#define T2CTCR         (*(volatile unsigned long *)(TMR2_BASE_ADDR + 0x70))

/* Timer 3 */
#define TMR3_BASE_ADDR		0xE0074000
#define T3IR           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x00))
#define T3TCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x04))
#define T3TC           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x08))
#define T3PR           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x0C))
#define T3PC           (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x10))
#define T3MCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x14))
#define T3MR0          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x18))
#define T3MR1          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x1C))
#define T3MR2          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x20))
#define T3MR3          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x24))
#define T3CCR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x28))
#define T3CR0          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x2C))
#define T3CR1          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x30))
#define T3CR2          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x34))
#define T3CR3          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x38))
#define T3EMR          (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x3C))
#define T3CTCR         (*(volatile unsigned long *)(TMR3_BASE_ADDR + 0x70))

/* Pulse Width Modulator (PWM) */
#define PWM0_BASE_ADDR		0xE0014000
#define PWM0IR          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x00))
#define PWM0TCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x04))
#define PWM0TC          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x08))
#define PWM0PR          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x0C))
#define PWM0PC          (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x10))
#define PWM0MCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x14))
#define PWM0MR0         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x18))
#define PWM0MR1         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x1C))
#define PWM0MR2         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x20))
#define PWM0MR3         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x24))
#define PWM0CCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x28))
#define PWM0CR0         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x2C))
#define PWM0CR1         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x30))
#define PWM0CR2         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x34))
#define PWM0CR3         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x38))
#define PWM0EMR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x3C))
#define PWM0MR4         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x40))
#define PWM0MR5         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x44))
#define PWM0MR6         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x48))
#define PWM0PCR         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x4C))
#define PWM0LER         (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x50))
#define PWM0CTCR        (*(volatile unsigned long *)(PWM0_BASE_ADDR + 0x70))

#define PWM1_BASE_ADDR		0xE0018000
#define PWM1IR          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x00))
#define PWM1TCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x04))
#define PWM1TC          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x08))
#define PWM1PR          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x0C))
#define PWM1PC          (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x10))
#define PWM1MCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x14))
#define PWM1MR0         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x18))
#define PWM1MR1         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x1C))
#define PWM1MR2         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x20))
#define PWM1MR3         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x24))
#define PWM1CCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x28))
#define PWM1CR0         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x2C))
#define PWM1CR1         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x30))
#define PWM1CR2         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x34))
#define PWM1CR3         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x38))
#define PWM1EMR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x3C))
#define PWM1MR4         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x40))
#define PWM1MR5         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x44))
#define PWM1MR6         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x48))
#define PWM1PCR         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x4C))
#define PWM1LER         (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x50))
#define PWM1CTCR        (*(volatile unsigned long *)(PWM1_BASE_ADDR + 0x70))


/* Universal Asynchronous Receiver Transmitter 0 (UART0) */
#define UART0_BASE_ADDR		0xE000C000
#define U0RBR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define U0THR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define U0DLL          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x00))
#define U0DLM          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
#define U0IER          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x04))
#define U0IIR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))
#define U0FCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x08))
#define U0LCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x0C))
#define U0LSR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x14))
#define U0SCR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x1C))
#define U0ACR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x20))
#define U0ICR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x24))
#define U0FDR          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x28))
#define U0TER          (*(volatile unsigned long *)(UART0_BASE_ADDR + 0x30))

/* Universal Asynchronous Receiver Transmitter 1 (UART1) */
#define UART1_BASE_ADDR		0xE0010000
#define U1RBR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define U1THR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define U1DLL          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x00))
#define U1DLM          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x04))
#define U1IER          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x04))
#define U1IIR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x08))
#define U1FCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x08))
#define U1LCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x0C))
#define U1MCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x10))
#define U1LSR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x14))
#define U1MSR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x18))
#define U1SCR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x1C))
#define U1ACR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x20))
#define U1FDR          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x28))
#define U1TER          (*(volatile unsigned long *)(UART1_BASE_ADDR + 0x30))

/* Universal Asynchronous Receiver Transmitter 2 (UART2) */
#define UART2_BASE_ADDR		0xE0078000
#define U2RBR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define U2THR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define U2DLL          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x00))
#define U2DLM          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x04))
#define U2IER          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x04))
#define U2IIR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x08))
#define U2FCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x08))
#define U2LCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x0C))
#define U2LSR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x14))
#define U2SCR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x1C))
#define U2ACR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x20))
#define U2ICR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x24))
#define U2FDR          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x28))
#define U2TER          (*(volatile unsigned long *)(UART2_BASE_ADDR + 0x30))

/* Universal Asynchronous Receiver Transmitter 3 (UART3) */
#define UART3_BASE_ADDR		0xE007C000
#define U3RBR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define U3THR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define U3DLL          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x00))
#define U3DLM          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x04))
#define U3IER          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x04))
#define U3IIR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x08))
#define U3FCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x08))
#define U3LCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x0C))
#define U3LSR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x14))
#define U3SCR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x1C))
#define U3ACR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x20))
#define U3ICR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x24))
#define U3FDR          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x28))
#define U3TER          (*(volatile unsigned long *)(UART3_BASE_ADDR + 0x30))

/* I2C Interface 0 */
#define I2C0_BASE_ADDR		0xE001C000
#define I2C0CONSET      (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x00))
#define I2C0STAT        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x04))
#define I2C0DAT         (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x08))
#define I2C0ADR         (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x0C))
#define I2C0SCLH        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x10))
#define I2C0SCLL        (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x14))
#define I2C0CONCLR      (*(volatile unsigned long *)(I2C0_BASE_ADDR + 0x18))

/* I2C Interface 1 */
#define I2C1_BASE_ADDR		0xE005C000
#define I2C1CONSET      (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x00))
#define I2C1STAT        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x04))
#define I2C1DAT         (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x08))
#define I2C1ADR         (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x0C))
#define I2C1SCLH        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x10))
#define I2C1SCLL        (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x14))
#define I2C1CONCLR      (*(volatile unsigned long *)(I2C1_BASE_ADDR + 0x18))

/* I2C Interface 2 */
#define I2C2_BASE_ADDR		0xE0080000
#define I2C2CONSET      (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x00))
#define I2C2STAT        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x04))
#define I2C2DAT         (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x08))
#define I2C2ADR         (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x0C))
#define I2C2SCLH        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x10))
#define I2C2SCLL        (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x14))
#define I2C2CONCLR      (*(volatile unsigned long *)(I2C2_BASE_ADDR + 0x18))




/* Real Time Clock */
#define RTC_BASE_ADDR		0xE0024000
#define RTC_ILR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x00))
#define RTC_CTC         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x04))
#define RTC_CCR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x08))
#define RTC_CIIR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x0C))
#define RTC_AMR         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x10))
#define RTC_CTIME0      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x14))
#define RTC_CTIME1      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x18))
#define RTC_CTIME2      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x1C))
#define RTC_SEC         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x20))
#define RTC_MIN         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x24))
#define RTC_HOUR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x28))
#define RTC_DOM         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x2C))
#define RTC_DOW         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x30))
#define RTC_DOY         (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x34))
#define RTC_MONTH       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x38))
#define RTC_YEAR        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x3C))
#define RTC_CISS        (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x40))
#define RTC_ALSEC       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x60))
#define RTC_ALMIN       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x64))
#define RTC_ALHOUR      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x68))
#define RTC_ALDOM       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x6C))
#define RTC_ALDOW       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x70))
#define RTC_ALDOY       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x74))
#define RTC_ALMON       (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x78))
#define RTC_ALYEAR      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x7C))
#define RTC_PREINT      (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x80))
#define RTC_PREFRAC     (*(volatile unsigned long *)(RTC_BASE_ADDR + 0x84))


/* A/D Converter 0 (AD0) */
#define AD0_BASE_ADDR		0xE0034000
#define AD0CR          (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x00))
#define AD0GDR         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x04))
#define AD0INTEN       (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x0C))
#define AD0DR0         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x10))
#define AD0DR1         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x14))
#define AD0DR2         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x18))
#define AD0DR3         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x1C))
#define AD0DR4         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x20))
#define AD0DR5         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x24))
#define AD0DR6         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x28))
#define AD0DR7         (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x2C))
#define AD0STAT        (*(volatile unsigned long *)(AD0_BASE_ADDR + 0x30))


/* D/A Converter */
#define DAC_BASE_ADDR		0xE006C000
#define DACR           (*(volatile unsigned long *)(DAC_BASE_ADDR + 0x00))


/* Watchdog */
#define WDG_BASE_ADDR		0xE0000000
#define WDMOD          (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x00))
#define WDTC           (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x04))
#define WDFEED         (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x08))
#define WDTV           (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x0C))
#define WDCLKSEL       (*(volatile unsigned long *)(WDG_BASE_ADDR + 0x10))


/* MultiMedia Card Interface(MCI) Controller */
#define MCI_BASE_ADDR		0xE008C000
#define MCI_POWER      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x00))
#define MCI_CLOCK      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x04))
#define MCI_ARGUMENT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x08))
#define MCI_COMMAND    (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x0C))
#define MCI_RESP_CMD   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x10))
#define MCI_RESP0      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x14))
#define MCI_RESP1      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x18))
#define MCI_RESP2      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x1C))
#define MCI_RESP3      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x20))
#define MCI_DATA_TMR   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x24))
#define MCI_DATA_LEN   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x28))
#define MCI_DATA_CTRL  (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x2C))
#define MCI_DATA_CNT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x30))
#define MCI_STATUS     (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x34))
#define MCI_CLEAR      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x38))
#define MCI_MASK0      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x3C))
#define MCI_MASK1      (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x40))
#define MCI_FIFO_CNT   (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x48))
#define MCI_FIFO       (*(volatile unsigned long *)(MCI_BASE_ADDR + 0x80))


/* I2S Interface Controller (I2S) */
#define I2S_BASE_ADDR		0xE0088000
#define I2S_DAO        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x00))
#define I2S_DAI        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x04))
#define I2S_TX_FIFO    (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x08))
#define I2S_RX_FIFO    (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x0C))
#define I2S_STATE      (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x10))
#define I2S_DMA1       (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x14))
#define I2S_DMA2       (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x18))
#define I2S_IRQ        (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x1C))
#define I2S_TXRATE     (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x20))
#define I2S_RXRATE     (*(volatile unsigned long *)(I2S_BASE_ADDR + 0x24))


/* General-purpose DMA Controller */
#define DMA_BASE_ADDR		0xFFE04000
#define GPDMA_INT_STAT         (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x000))
#define GPDMA_INT_TCSTAT       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x004))
#define GPDMA_INT_TCCLR        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x008))
#define GPDMA_INT_ERR_STAT     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x00C))
#define GPDMA_INT_ERR_CLR      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x010))
#define GPDMA_RAW_INT_TCSTAT   (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x014))
#define GPDMA_RAW_INT_ERR_STAT (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x018))
#define GPDMA_ENABLED_CHNS     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x01C))
#define GPDMA_SOFT_BREQ        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x020))
#define GPDMA_SOFT_SREQ        (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x024))
#define GPDMA_SOFT_LBREQ       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x028))
#define GPDMA_SOFT_LSREQ       (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x02C))
#define GPDMA_CONFIG           (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x030))
#define GPDMA_SYNC             (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x034))

/* DMA channel 0 registers */
#define GPDMA_CH0_SRC      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x100))
#define GPDMA_CH0_DEST     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x104))
#define GPDMA_CH0_LLI      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x108))
#define GPDMA_CH0_CTRL     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x10C))
#define GPDMA_CH0_CFG      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x110))

/* DMA channel 1 registers */
#define GPDMA_CH1_SRC      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x120))
#define GPDMA_CH1_DEST     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x124))
#define GPDMA_CH1_LLI      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x128))
#define GPDMA_CH1_CTRL     (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x12C))
#define GPDMA_CH1_CFG      (*(volatile unsigned long *)(DMA_BASE_ADDR + 0x130))

//CAN USB and MAC has been removed as not needed for osiFC

/* Some bit-mask definitions - mthomas */

/* Timers */
#define TxIR_MR0_Interrupt (1<<0)
#define TxIR_MR1_Interrupt (1<<1)
#define TxIR_MR2_Interrupt (1<<2)
#define TxIR_MR3_Interrupt (1<<3)
#define TxIR_CR0_Interrupt (1<<4)
#define TxIR_CR1_Interrupt (1<<5)
#define TxIR_CR2_Interrupt (1<<6)
#define TxIR_CR3_Interrupt (1<<7)

#define TxTCR_Counter_Enable (1<<0)
#define TxTCR_Counter_Reset  (1<<1)

#define TxMCR_MR0I  (1<<0)
#define TxMCR_MR0R  (1<<1)
#define TxMCR_MR0S  (1<<2)
#define TxMCR_MR1I  (1<<3)
#define TxMCR_MR1R  (1<<4)
#define TxMCR_MR1S  (1<<5)
#define TxMCR_MR2I  (1<<6)
#define TxMCR_MR2R  (1<<7)
#define TxMCR_MR2S  (1<<8)
#define TxMCR_MR3I  (1<<9)
#define TxMCR_MR3R  (1<<10)
#define TxMCR_MR3S  (1<<11)

/* VIC */
#define VIC_CHAN_NUM_WDT      0
#define VIC_CHAN_NUM_UNUSED   1
#define VIC_CHAN_NUM_ARM_Core_DgbCommRX 2
#define VIC_CHAN_NUM_ARM_Core_DbgCommTX 3
#define VIC_CHAN_NUM_Timer0   4
#define VIC_CHAN_NUM_Timer1   5
#define VIC_CHAN_NUM_UART0    6
#define VIC_CHAN_NUM_UART1    7
#define VIC_CHAN_NUM_PWM1     8
#define VIC_CHAN_NUM_I2C0     9
#define VIC_CHAN_NUM_SPI     10
#define VIC_CHAN_NUM_SSP0    10
#define VIC_CHAN_NUM_SSP1    11
#define VIC_CHAN_NUM_PLL     12
#define VIC_CHAN_NUM_RTC     13
#define VIC_CHAN_NUM_EINT0   14
#define VIC_CHAN_NUM_EINT1   15
#define VIC_CHAN_NUM_EINT2   16
#define VIC_CHAN_NUM_EINT3   17
#define VIC_CHAN_NUM_ADC0    18
#define VIC_CHAN_NUM_I2C1    19
#define VIC_CHAN_NUM_BOD     20
#define VIC_CHAN_NUM_Ethernet 21
#define VIC_CHAN_NUM_USB     22
#define VIC_CHAN_NUM_CAN     23
#define VIC_CHAN_NUM_SD_MMC  24
#define VIC_CHAN_NUM_GP_DMA  25
#define VIC_CHAN_NUM_Timer2  26
#define VIC_CHAN_NUM_Timer3  27
#define VIC_CHAN_NUM_UART2   28
#define VIC_CHAN_NUM_UART3   29
#define VIC_CHAN_NUM_I2C2    30
#define VIC_CHAN_NUM_I2S     31

#define VIC_SIZE		32

#define VIC_CHAN_TO_MASK(vctm_chan_num__) (1<<vctm_chan_num__)
#define VIC_CHAN_TO_MASKLOW(vctm_chan_num__) (0<<vctm_chan_num__)
#endif  //__LPC23xX_H

