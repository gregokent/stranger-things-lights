#include <Adafruit_NeoPixel.h>

#define DATA_PIN 7
#define NUM_LEDS 50
#define COLOR_ORDER RGB

Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_RGB + NEO_KHZ800);

void setup()
{
  strip.begin();
  strip.setBrightness(1);
  strip.show();
}

void loop()
{
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
}

void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
