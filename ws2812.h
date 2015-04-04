#include <stdint.h>

// Transmit codes
#define HIGH_CODE	(0xF0)			// b11110000
#define LOW_CODE	(0xC0)			// b11000000

class WS2812
{
public:
    // Configure processor to output to data strip
    WS2812(uint16_t numLeds, uint8_t pin);

    // Send colors to the strip and show them. Disables interrupts while processing.
    void show(void);

    // Set the color of a certain LED
    void setLEDColour(uint16_t p, uint8_t r, uint8_t g, uint8_t b);

    // Clear the color of all LEDs (make them black/off)
    void clear(void);

    // Fill the strip with a solid color. This will update the strip.
    void fill(uint8_t r, uint8_t g, uint8_t b);

    // Pack RGB values into an int
    uint32_t  Colour(uint8_t r, uint8_t g, uint8_t b);

    // Set a pixel to a packed colour
    void setLEDColour(uint16_t p, uint32_t c);

private:
    uint16_t pin;
    uint16_t numLeds;
    uint16_t numBytes;
    uint8_t *leds;
};
    
