#include <FastLED.h>

#define DATA_PIN 3
#define NUM_LEDS 35

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
  FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(128);
  initializeDefaults();
  showDefault();
}

void loop()
{
  // Delay between crazy stuff happening.
  delay(random(15, 30) * 1000);
  int behavior = random(0, 7);
  switch (behavior)
  {
  case 0:
    message("im here");
    break;
  case 1:
    message("run");
    break;
  case 2:
    message("help");
    break;
  case 3:
    message("get out");
    break;
  case 4:
    message("hide");
    break;
  case 5:
    redpulse();
    break;
  case 6:
    sparkleOut();
    break;
  default:
    showDefault();
    break;
  }
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
    delay(100);
  }

  delay(1000);
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
  delay(3000);
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
    delay(ms / fadeSteps);
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
    delay(ms / fadeSteps);
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
      delay(1000);
      FastLED.clear();
      FastLED.show();
      delay(250);
    }
    else
    {
      // Space, punctuation, etc.
      delay(1500);
    }
  }

  delay(1000);
  fadeToDefault(10, 1000);
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
    return 33;
    break;
  case 'b':
    return 32;
    break;
  case 'c':
    return 31;
    break;
  case 'd':
    return 30;
    break;
  case 'e':
    return 29;
    break;
  case 'f':
    return 28;
    break;
  case 'g':
    return 27;
    break;
  case 'h':
    return 26;
    break;
  case 'i':
    return 14;
    break;
  case 'j':
    return 16;
    break;
  case 'k':
    return 17;
    break;
  case 'l':
    return 18;
    break;
  case 'm':
    return 19;
    break;
  case 'n':
    return 20;
    break;
  case 'o':
    return 21;
    break;
  case 'p':
    return 22;
    break;
  case 'q':
    return 24;
    break;
  case 'r':
    return 11;
    break;
  case 's':
    return 9;
    break;
  case 't':
    return 8;
    break;
  case 'u':
    return 7;
    break;
  case 'v':
    return 6;
    break;
  case 'w':
    return 5;
    break;
  case 'x':
    return 4;
    break;
  case 'y':
    return 3;
    break;
  case 'z':
    return 1;
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
}
