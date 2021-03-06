#include <msp430.h>
#include "ws2812.h"
#include <Energia.h>
#include <string.h>
#include <stdlib.h>

// Initializes everything needed to use this library. This clears the strip.
WS2812::WS2812(uint16_t n, uint8_t p) :
	numLeds(n),
	pin(p),
	numBytes(n*3),
	leds(NULL)
{

	if ((leds = (uint8_t *)malloc(numBytes)) != NULL) {
		memset(leds, 0, numBytes);
	}
	
	UCB0CTL1 = UCSWRST | UCSSEL_2; 
	pinMode_int(pin, SPIMOSI_SET_MODE);

	UCB0CTL0 =  UCCKPH | UCMSB | UCMST | UCSYNC; 	// 3-pin, MSB, 8-bit SPI master

//	P1SEL |= pin;		// configure output pin as SPI output
//	P1SEL2 |= pin;
//	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC; 	// 3-pin, MSB, 8-bit SPI master
//	UCB0CTL1 |= UCSSEL_2;   // SMCLK source (16 MHz)
	UCB0BR0 = 3;		// 16 MHz / 3 = .1875 us per bit
	UCB0BR1 = 0;
	UCB0CTL1 &= ~UCSWRST; // Initialize USCI state machine

}

// Sets the colour of a certain LED (0 indexed)
void WS2812::setLEDColour(uint16_t p, uint8_t r, uint8_t g, uint8_t b)
{
  if (p < numLeds) {
	  p *= 3;
	  leds[p] = (g * brightness) >> 8;
	  leds[p + 1] = (r * brightness) >> 8;
	  leds[p + 2] = (b * brightness) >> 8;
  }
}

void WS2812::setLEDColour(uint16_t p, uint32_t c)
{
	uint8_t r, g, b;
	r = (uint8_t)(c >> 16);
	g = (uint8_t)(c >> 8);
	b = (uint8_t)c;
	setLEDColour(p, r, g, b);
}

uint32_t WS2812::Colour(uint8_t r, uint8_t g, uint8_t b)
{
	return  ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Send colours to the strip and show them. Disables interrupts while processing.
void WS2812::show(void) {
  __bic_SR_register(GIE);       	// disable interrupts
  uint8_t *p;
	
  // send RGB color for every LED
  for (p = leds; p < &leds[numBytes]; p++) {
    uint8_t mask = 0x80;					// b1000000

    // check each of the 8 bits
    while (mask != 0){
      while (!(IFG2 & UCB0TXIFG));	// wait to transmit
      if (*p & mask){			// most significant bit first
	UCB0TXBUF = HIGH_CODE;		// send 1
      } else {
	UCB0TXBUF = LOW_CODE;		// send 0
      }
      mask >>= 1;				// check next bit
    }
  }

  // send RES code for at least 50 us (800 cycles at 16 MHz)
  __delay_cycles(800);

  __bis_SR_register(GIE);       	// enable interrupts
}

// Clear the color of all LEDs (make them black/off)
void WS2812::clear(){
  fill(0x00, 0x00, 0x00);	// black
}

// Fill the strip with a solid color. This will update the strip.
void WS2812::fill(uint8_t r, uint8_t g, uint8_t b){
  unsigned int i;
  for (i = 0; i < numLeds; i++) {
    setLEDColour(i, r, g, b);		// set all LEDs to specified color
  }
  show();			    // refresh strip
}

uint16_t WS2812::numPixels(void) {
  return numLeds;
}

uint8_t WS2812::getBrightness(void) {
  return brightness - 1;
}

void WS2812::setBrightness(uint8_t b) {
  uint8_t oldBrightness = brightness - 1;

  b += 1;

  if (b != brightness) {
    uint16_t scale;
    if (oldBrightness == 0)
      scale = 0;
    else if (b == 0) {
      scale = 65535 / oldBrightness;
    } else {
      scale = (uint16_t) ((b << 8) - 1) / oldBrightness;
    }
    for (int i = 0; i < numBytes; i++)
      leds[i] = (leds[i] * scale) >> 8;
    brightness = b;
  }
}
