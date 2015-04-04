#include <stdint.h>

// Transmit codes
#define HIGH_CODE	(0xF0)			// b11110000
#define LOW_CODE	(0xC0)			// b11000000

class WS2812
{
public:
    // Configure processor to output to data strip
    WS2812(uint16_t numLeds, uint8_t pin);

    // Send colours to the strip and show them.
    // Disables interrupts while processing. 
    void show(void);

    // Set the colour of a certain LED
    void setLEDColour(uint16_t p, uint8_t r, uint8_t g, uint8_t b);

    // Clear the colour of all LEDs (make them black/off)
    void clear(void);

    // Fill the strip with a solid colour. This will update the strip.
    void fill(uint8_t r, uint8_t g, uint8_t b);

    // Pack RGB values into an int
    static uint32_t Colour(uint8_t r, uint8_t g, uint8_t b);

    // Set a pixel to a packed colour
    void setLEDColour(uint16_t p, uint32_t c);

    uint32_t getLEDColour(uint16_t p);

    // Get/Set brightness
    uint8_t getBrightness(void);
    void setBrightness(uint8_t);

    uint16_t numPixels(void);

// Compatibility with AdaFruit NeoPixel library
    inline void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
        setLEDColour(n, r, g, b);
    }

    inline void setPixelColor(uint16_t n, uint32_t c) {
        setLEDColour(n, c);
    }

    static inline uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
        return Colour(r, g, b);
    }

    inline uint32_t getPixelColor(uint16_t n) {
        return getLEDColour(n);
    }

private:
    uint16_t numLeds;
    uint16_t pin;
    uint16_t numBytes;
    uint8_t brightness;
    uint8_t *leds;
};

