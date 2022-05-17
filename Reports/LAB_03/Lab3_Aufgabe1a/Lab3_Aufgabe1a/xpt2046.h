// XPT2046 resistive touch panel driver
//
//  2017-04-17 Uwe Zimmermann

#ifndef XPT2046_H
#define XPT2046_H

#include <avr/io.h>

//#define XPT2046_SOFTSPI

#define XPT2046_CS_PORT PORTC
#define XPT2046_CS_DDR  DDRC
#define XPT2046_CS_PIN  PINC
#define XPT2046_CS      PC3
#define XPT2046_CS_HI() XPT2046_CS_PORT |= (1 << XPT2046_CS)
#define XPT2046_CS_LO() XPT2046_CS_PORT &= ~(1 << XPT2046_CS)

#ifdef XPT2046_SOFTSPI

#define XPT2046_CLK_PORT PORTD
#define XPT2046_CLK_DDR  DDRD
#define XPT2046_CLK_PIN  PIND
#define XPT2046_CLK      PD7
#define XPT2046_CLK_HI() XPT2046_CLK_PORT |= (1 << XPT2046_CLK)
#define XPT2046_CLK_LO() XPT2046_CLK_PORT &= ~(1 << XPT2046_CLK)

#define XPT2046_DI_PORT PORTD
#define XPT2046_DI_DDR  DDRD
#define XPT2046_DI_PIN  PIND
#define XPT2046_DI      PD6
#define XPT2046_DI_HI() XPT2046_DI_PORT |= (1 << XPT2046_DI)
#define XPT2046_DI_LO() XPT2046_DI_PORT &= ~(1 << XPT2046_DI)

#define XPT2046_DO_PORT PORTD
#define XPT2046_DO_DDR  DDRD
#define XPT2046_DO_PIN  PIND
#define XPT2046_DO      PD5
#define XPT2046_DO_STATE() ((XPT2046_DO_PIN & (1 << XPT2046_DO)) >> XPT2046_DO)

#endif // SOFTSPI

// 12 bit modes
//                             AAAMS
#define XPT2046_MODE_S_TEMP0 0b00001
#define XPT2046_MODE_S_XPYM  0b00101
#define XPT2046_MODE_S_BATT  0b01001
#define XPT2046_MODE_S_XPZ1  0b01101
#define XPT2046_MODE_S_YNZ2  0b10001
#define XPT2046_MODE_S_YPXM  0b10101
#define XPT2046_MODE_S_AUX   0b11001
#define XPT2046_MODE_S_TEMP1 0b11101

#define XPT2046_MODE_D_XP    0b00100   // REF+ YP  REF- YN
#define XPT2046_MODE_D_XPYP  0b01100   // REF+ YP  REF- XN
#define XPT2046_MODE_D_YNYP  0b10000   // REF+ YP  REF- XN
#define XPT2046_MODE_D_YP    0b10100   // REF+ XP  REF- XN

#define XPT2046_PD_POWERDOWN 0b00
#define XPT2046_PD_REFOFF    0b01
#define XPT2046_PD_ADCOFF    0b10
#define XPT2046_PD_ON        0b11      // Interrupt disabled!

uint16_t xpt2046_xfer(uint8_t cmd, uint8_t handleCS);

uint16_t xpt2046_sample(uint8_t mode, uint8_t PD, uint8_t handleCS);

void xpt2046_nsample(uint16_t *xval, uint16_t *yval, uint8_t oversample);

void xpt2046_init();

#endif // XPT2046_H