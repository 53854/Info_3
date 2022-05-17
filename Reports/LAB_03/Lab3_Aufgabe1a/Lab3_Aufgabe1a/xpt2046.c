// XPT2046 resistive touch panel driver
//
//  2017-04-17 Uwe Zimmermann

#include "xpt2046.h"

#ifdef XPT2046_SOFTSPI
uint16_t xpt2046_xfer(uint8_t cmd, uint8_t handleCS)
{
  uint8_t i;
  uint16_t dummy = 0;
  XPT2046_CS_HI();
  XPT2046_CLK_LO();
  XPT2046_DI_LO();
  XPT2046_CS_LO();   // start xfer
  for (i=0; i<8; i++)
  {
    if (cmd & 128)    // check MSB
    {
      XPT2046_DI_HI();
    }
    else
    {
      XPT2046_DI_LO();
    }
    cmd <<= 1;
    XPT2046_CLK_HI();
    XPT2046_CLK_LO();
  }
  // we don't have access to /BUSY
  // is the data valid on the falling clock edge? unclear from the datasheet, but probable
  for (i=0; i < 16; i++)
  {
    XPT2046_CLK_HI();
    dummy <<= 1;
    dummy |= XPT2046_DO_STATE();
    XPT2046_CLK_LO();
  }
  XPT2046_CS_HI();
  return dummy;
}

#else // hardware SPI

uint16_t xpt2046_xfer(uint8_t cmd, uint8_t handleCS)
{
  uint16_t dummy = 0;
  if (handleCS)
  {
    XPT2046_CS_LO();   // start xfer
  }
  SPDR = cmd;              //move data into spdr
  while (!(SPSR & (1<<SPIF)));  //wait until the transmission is finished
  SPDR = 0;              //move data into spdr
  while (!(SPSR & (1<<SPIF)));  //wait until the transmission is finished
  dummy = SPDR;
  SPDR = 0;              //move data into spdr
  while (!(SPSR & (1<<SPIF)));  //wait until the transmission is finished
  dummy <<= 8;
  dummy |= SPDR;
  if (handleCS)
  {
    XPT2046_CS_HI();
  }
  return dummy;
}

#endif // SOFTSPI

uint16_t xpt2046_sample(uint8_t mode, uint8_t PD, uint8_t handleCS)
{
  return xpt2046_xfer(0b10000000 | ((mode & 0b11111) << 2) | (PD & 0b011), handleCS);
}

void xpt2046_nsample(uint16_t *xval, uint16_t *yval, uint8_t oversample)
{
  uint8_t i;
  uint16_t dummy;
  XPT2046_CS_LO();   // start xfer
  dummy = 0;
  for (i=0; i<oversample; i++)
  {
    dummy += xpt2046_sample(XPT2046_MODE_D_XP, XPT2046_PD_ON, 0)>>4;
  }
  *xval = dummy / oversample;
  dummy = 0;
  for (i=0; i<oversample; i++)
  {
    if (i < (oversample-1))
    {
      dummy += xpt2046_sample(XPT2046_MODE_D_YP, XPT2046_PD_ON, 0)>>4;
    }
    else
    {
      dummy += xpt2046_sample(XPT2046_MODE_D_YP, XPT2046_PD_POWERDOWN, 0)>>4;
    }
  }
  *yval = dummy / oversample;
  XPT2046_CS_HI();
}

void xpt2046_init()
{
  
  XPT2046_CS_DDR  |= (1 << XPT2046_CS);
  
#ifdef XPT2046_SOFTSPI
  XPT2046_CLK_DDR |= (1 << XPT2046_CLK);
  XPT2046_DI_DDR  |= (1 << XPT2046_DI);
#else
  DDRB  |= (1<<1)|(1<<2)|(1<<3)|(1<<5); //CS,SS,MOSI,SCK as output(although SS will be unused throughout the program)
  SPCR   = (1<<SPE)|(1<<MSTR);          //mode 0,fosc/4
  SPSR  |= (1<<SPI2X);                  //doubling spi speed.i.e final spi speed-fosc/2
#endif  

  xpt2046_sample(XPT2046_MODE_D_YP, XPT2046_PD_POWERDOWN, 1);
}

