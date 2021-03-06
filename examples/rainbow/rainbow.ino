#include <msp430.h>
#include <ws2812.h>

#define PIN P1_7 /* P1.7 */
#define NUMLEDS (8)

WS2812 strip(PIN, NUMLEDS);

void setup()
{
	strip.clear();
}

// Prints a rainbow on the ENTIRE LED strip.
//  The rainbow begins at a specified position. 
// ROY G BIV!
void rainbow(byte startPosition) 
{
  // Need to scale our rainbow. We want a variety of colors, even if there
  // are just 10 or so pixels.
  int rainbowScale = 192 / NUMLEDS;
  
  // Next we setup each pixel with the right color
  for (int i=0; i< NUMLEDS; i++)
  {
    // There are 192 total colors we can get out of the rainbowOrder function.
    // It'll return a color between red->orange->green->...->violet for 0-191.
    strip.setLEDColour(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192));
  }
  // Finally, actually turn the Strip on:
  strip.show();
}

// Input a value 0 to 191 to get a color value.
// The colors are a transition red->yellow->green->aqua->blue->fuchsia->red...
//  Adapted from Wheel function in the Adafruit_NeoPixel library example sketch
uint32_t rainbowOrder(byte position) 
{
  // 6 total zones of color change:
  if (position < 31)  // Red -> Yellow (Red = FF, blue = 0, green goes 00-FF)
  {
    return strip.Colour(0xFF, position * 8, 0);
  }

  if (position < 63)  // Yellow -> Green (Green = FF, blue = 0, red goes FF->00)
  {
    position -= 31;
    return strip.Colour(0xFF - position * 8, 0xFF, 0);
  }

  if (position < 95)  // Green->Aqua (Green = FF, red = 0, blue goes 00->FF)
  {
    position -= 63;
    return strip.Colour(0, 0xFF, position * 8);
  }
  if (position < 127)  // Aqua->Blue (Blue = FF, red = 0, green goes FF->00)
  {
    position -= 95;
    return strip.Colour(0, 0xFF - position * 8, 0xFF);
  }

  if (position < 159)  // Blue->Fuchsia (Blue = FF, green = 0, red goes 00->FF)
  {
    position -= 127;
    return strip.Colour(position * 8, 0, 0xFF);
  }
//160 <position< 191   Fuchsia->Red (Red = FF, green = 0, blue goes FF->00)
    position -= 159;
    return strip.Colour(0xFF, 0x00, 0xFF - position * 8);
}

void loop()
{
  for (int i=0; i<NUMLEDS*10; i++)
  {
    rainbow(i);
    delay(100);  // Delay between rainbow slides
  }

}
