#include <FastLED.h>

#define DATA_PIN 3
#define NUM_LEDS 50

CRGB leds[NUM_LEDS];

void setup()
{
  // Holiday Coro tells me the chip driving the lights is a UCS1903
  // but the sticker on the strand says 2811/1903. Hmmmm. The UCS1903
  // setting works.
  //
  // The strand also needs a 5V data signal. If you have the power
  // plugged into the wall _while_ you're programming, all works
  // well. If it's only plugged into USB, the data signal comes out
  // at 3.3V and the strand goes haywire. Took me a full day to
  // figure this out. Plug the stupid board into wall power.
  FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop()
{
  colorWipe(CRGB::Red, 50);
  colorWipe(CRGB::Blue, 50);
  colorWipe(CRGB::Green, 50);
}

void colorWipe(CRGB color, int wait) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(wait);
  }
}
