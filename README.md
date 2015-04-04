MSP430 NeoPixel/WS2812 C Library
================================

An easy to use library for controlling WS2812 LEDs (branded by
Adafruit as NeoPixels) with an MSP430 microcontroller. The library is
syntactically based off of Adafruit's NeoPixel library; it was adapted
for the Energia framework by Peter Chubb.

## Instructions

Use of this library is simple. Put this directory in the directory
called `libraries' in your Energia sketchbook directory.

Set your CPU's clock speed to **16 MHz**.

Initialise the class with

WS2812 strip = WS2812(NUM_LEDS, PIN)

PIN will usually be P1_7 on the MSP430g2553.


Call **setLEDColour()** to change the colour of a specific pixel, 0
indexed from the start of the strip.

To actually display your colours, you need to call
**show()**. This function temporarily disables interrupts to
ensure timing requirements are kept. 

Connect the data line for your strip to the B SIMO pin for your
MSP430. This library is written with the belief that this pin is 1.7,
which it is on the Launchpad. Modify the define for **OUTPUT_PIN** and
make sure the code in initStrip() is for the correct port if your
processor differs. Don't forget to attach a 300-500 Ohm resistor on
the data line.  Also remember the Launchpad runs at 3.3V; most strips
need 5V.

## Important Notes

#### SPI Note

This software library drives the LEDs by taking advantage of the SPI
module, which is essentially the best option for programming in C. The
same functionality can be achieved using assembly, but this is not the
focus of this library. This means that you are required to dedicate a
SPI line to your strip. This library is written to use the SPI on USCI
B.

#### Clock Speed

The timing requirements for the WS2812 LEDs are very tight. As such,
this requires a relatively high speed SPI clock (this uses ~5.3 MHz)
and an even higher CPU clock. **Set the DCO clock to 16 MHz to use
this library**. If you wish to use a different clock speed (you can
use higher as long as the SPI clock can be modulated to an appropiate
frequency), you will have to modify the the SPI clock's modulation in
the initialisation function.

#### Device Compatibility

The library was written to work on the MSP430G2553, which comes with
the G2 Launchpad. The code should be able to be modified to work on
most other MSP430 devices with a clock speed of at least 16 MHz. This
will possibly require you to modify the SPI configuration in
the constructor. 
