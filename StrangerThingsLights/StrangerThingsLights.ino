#include <FastLED.h>

#define DATA_PIN D2
#define NUM_LEDS 50

CRGB leds[NUM_LEDS];
CRGB defaultColors[NUM_LEDS];

void setup()
{
  // Holiday Coro tells me the chip driving the lights is a UCS1903 but the
  // sticker on the strand says 2811/1903. Hmmmm. The UCS1903 setting works.
  //
  // The strand also needs a 5V data signal. If you have the power plugged into
  // the wall _while_ you're programming, all works well. If it's only plugged
  // into USB, the data signal comes out at 3.3V and the strand goes haywire.
  // Took me a full day to figure this out. Plug the stupid board into wall
  // power.
  randomSeed(analogRead(0));
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  initializeDefaults();
  showDefault();
}

void loop()
{
  // Delay between crazy stuff happening.
      // message("abcdefghijklmnopqrstuvwxyz");
  FastLED.delay(random(5, 10) * 1000);
  int behavior = random(0, 8);
  switch (behavior)
  {
  case 0:
    message("right here");
    break;
  case 1:
    message("trick or treat");
    break;
  case 2:
    message("help");
    break;
  case 3:
    message("run");
    break;
  case 4:
    message("hide");
    break;
  case 5:
    message("happy halloween leonard leopards");
    break;
  case 6:
    redpulse();
    break;
  case 7:
    sparkleOut();
    break;
  case 8:
    sparkle(7000);
    break;
  default:
    showDefault();
    break;
  }
}

void sparkle(int duration)
{
  int start = millis();
  while( millis() - start < duration) {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      int on = random(0,10);
      leds[i] = on < 7 ? defaultColors[i] : CRGB::Black;

    }
      FastLED.show();
      FastLED.delay(100);
  }

  sparkleOut();
  FastLED.delay(1000);
  fadeToDefault(10, 1000);
}

void sparkleOut()
{
  // Create a list of light indices.
  int indices[NUM_LEDS];
  for(int i = 0; i < NUM_LEDS; i++)
  {
    indices[i] = i;
  }

  // Shuffle the indices into a random order.
  int last = 0;
  int temp = indices[last];
  for (int i = 0; i < NUM_LEDS; i++)
  {
    int index = random(NUM_LEDS);
    indices[last] = indices[index];
    last = index;
  }

  indices[last] = temp;

  // Turn the lights off in the random order.
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[indices[i]] = CRGB::Black;
    FastLED.show();
    FastLED.delay(70);
  }

  FastLED.delay(1000);
  fadeToDefault(10, 1000);
}

void redpulse()
{
  for (int i = 0; i < 3; i++)
  {
    fadeToColor(CRGB::Red, 20, 1000);
    fadeToColor(CRGB(64, 0, 0), 10, 500);
  }

  fadeToColor(CRGB::Red, 20, 1000);
  FastLED.delay(3000);
  fadeToDefault(20, 1000);
}

void fadeToDefault(int fadeSteps, int ms)
{
  CRGB original[NUM_LEDS];
  for (int i = 0; i < NUM_LEDS; i++)
  {
    original[i] = CRGB(leds[i]);
  }

  for (int step = 1; step <= fadeSteps; step++)
  {
    // blend 'amount' is a fraction from 0 - 255
    float amount = ((float)step / fadeSteps) * 255;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = blend(original[i], defaultColors[i], amount);
    }

    FastLED.show();
    FastLED.delay(ms / fadeSteps);
  }
}

void fadeToColor(CRGB color, int fadeSteps, int ms)
{
  CRGB original[NUM_LEDS];
  for (int i = 0; i < NUM_LEDS; i++)
  {
    original[i] = CRGB(leds[i]);
  }

  for (int step = 1; step <= fadeSteps; step++)
  {
    // blend 'amount' is a fraction from 0 - 255
    float amount = ((float)step / fadeSteps) * 255;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = blend(original[i], color, amount);
    }

    FastLED.show();
    FastLED.delay(ms / fadeSteps);
  }
}

void message(char *text)
{
  // A couple of "pops" of color before showing the message.
  fadeToColor(CRGB::Black, 10, 250);
  fadeToColor(CRGB::White, 10, 250);
  fadeToColor(CRGB::Black, 5, 100);
  fadeToColor(CRGB::White, 10, 500);
  fadeToColor(CRGB::Black, 10, 500);
  size_t length = strlen(text);
  for (int i = 0; i < length; i++)
  {
    int index = lookupChar(text[i]);
    if (index >= 0)
    {
      leds[index] = CRGB(defaultColors[index]);
      FastLED.show();
      FastLED.delay(1250);
      FastLED.clear();
      FastLED.show();
      FastLED.delay(500);
    }
    else
    {
      // Space, punctuation, etc.
      FastLED.delay(1250);
    }
  }

  FastLED.delay(1000);
  fadeToDefault(20, 1000);
}

void showDefault()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(defaultColors[i]);
  }

  FastLED.show();
}

int lookupChar(char c)
{
  switch (c)
  {
  case 'a':
    return 47;
    break;
  case 'b':
    return 46;
    break;
  case 'c':
    return 44;
    break;
  case 'd':
    return 42;
    break;
  case 'e':
    return 40;
    break;
  case 'f':
    return 39;
    break;
  case 'g':
    return 37;
    break;
  case 'h':
    return 36;
    break;
  case 'i':
    return 19;
    break;
  case 'j':
    return 21;
    break;
  case 'k':
    return 22;
    break;
  case 'l':
    return 24;
    break;
  case 'm':
    return 25;
    break;
  case 'n':
    return 27;
    break;
  case 'o':
    return 28;
    break;
  case 'p':
    return 30;
    break;
  case 'q':
    return 32;
    break;
  case 'r':
    return 16;
    break;
  case 's':
    return 14;
    break;
  case 't':
    return 12;
    break;
  case 'u':
    return 11;
    break;
  case 'v':
    return 10;
    break;
  case 'w':
    return 8;
    break;
  case 'x':
    return 6;
    break;
  case 'y':
    return 4;
    break;
  case 'z':
    return 2;
    break;
  default:
    return -1;
    break;
  }
}

void initializeDefaults()
{
  // I put physical C9 color glass bulbs over the LEDs for a more authentic
  // look. I also randomized how the C9 bulb colors were distributed. This means
  // the default color for each LED is different based on the color of the
  // glass; and that it's not evenly/programmatically distributed.
  //
  // Yellow bulbs - CRGB::Orange (yellow/orange colors seem to be hard for these LEDs)
  // Red bulbs - CRGB::Red
  // Blue bulbs - CRGB::Blue
  // Green bulbs - CRGB::Green
  // Purple bulbs - CRGB::Purple
  // Orange bulbs - CRGB::Orange
  defaultColors[0] = CRGB::Red;
  defaultColors[1] = CRGB::Blue;
  defaultColors[2] = CRGB::Yellow;
  defaultColors[3] = CRGB::Red;
  defaultColors[4] = CRGB::Green;
  defaultColors[5] = CRGB::Yellow;
  defaultColors[6] = CRGB::Blue;
  defaultColors[7] = CRGB::Orange;
  defaultColors[8] = CRGB::Purple;
  defaultColors[9] = CRGB::Green;
  defaultColors[10] = CRGB::Red;
  defaultColors[11] = CRGB::Yellow;
  defaultColors[12] = CRGB::Blue;
  defaultColors[13] = CRGB::Red;
  defaultColors[14] = CRGB::Green;
  defaultColors[15] = CRGB::Purple;
  defaultColors[16] = CRGB::Yellow;
  defaultColors[17] = CRGB::Red;
  defaultColors[18] = CRGB::Green;
  defaultColors[19] = CRGB::Orange;
  defaultColors[20] = CRGB::Yellow;
  defaultColors[21] = CRGB::Green;
  defaultColors[22] = CRGB::Blue;
  defaultColors[23] = CRGB::Purple;
  defaultColors[24] = CRGB::Red;
  defaultColors[25] = CRGB::Green;
  defaultColors[26] = CRGB::Yellow;
  defaultColors[27] = CRGB::Blue;
  defaultColors[28] = CRGB::White;
  defaultColors[29] = CRGB::Green;
  defaultColors[30] = CRGB::Yellow;
  defaultColors[31] = CRGB::Blue;
  defaultColors[32] = CRGB::Purple;
  defaultColors[33] = CRGB::Red;
  defaultColors[34] = CRGB::Yellow;
  defaultColors[35] = CRGB::Yellow;
  defaultColors[36] = CRGB::Green;
  defaultColors[37] = CRGB::Blue;
  defaultColors[38] = CRGB::Purple;
  defaultColors[39] = CRGB::Red;
  defaultColors[40] = CRGB::Green;
  defaultColors[41] = CRGB::Yellow;
  defaultColors[42] = CRGB::Blue;
  defaultColors[43] = CRGB::White;
  defaultColors[44] = CRGB::Green;
  defaultColors[45] = CRGB::Yellow;
  defaultColors[46] = CRGB::Blue;
  defaultColors[47] = CRGB::Purple;
  defaultColors[48] = CRGB::Red;
  defaultColors[49] = CRGB::Yellow;
}

  const CRGB strange_colors[] = {
    CRGB::Bisque, CRGB::MediumBlue, CRGB::Indigo, CRGB::GreenYellow, CRGB::DodgerBlue, CRGB::Goldenrod, CRGB::Indigo, CRGB::GreenYellow, 
    CRGB::GreenYellow, CRGB::Indigo, CRGB::MediumBlue, CRGB::GreenYellow, CRGB::Goldenrod, CRGB::Indigo, CRGB::Indigo, CRGB::GreenYellow, CRGB::Indigo,
    CRGB::GreenYellow, CRGB::Bisque, CRGB::Goldenrod, CRGB::MediumBlue, CRGB::Indigo, CRGB::MediumBlue, CRGB::Goldenrod, CRGB::Indigo, CRGB::Indigo
  };
