#include <PS2X_lib.h>

#include <FastLED.h>

#define DATA_PIN D2
#define NUM_LEDS 100

//CRGB leds[NUM_LEDS];
CRGB defaultColors[NUM_LEDS];

CRGBArray<NUM_LEDS> leds;

// Overall twinkle speed.
// 0 (VERY slow) to 8 (VERY fast).  
// 4, 5, and 6 are recommended, default is 4.
#define TWINKLE_SPEED 4

// Overall twinkle density.
// 0 (NONE lit) to 8 (ALL lit at once).  
// Default is 5.
#define TWINKLE_DENSITY 6

// How often to change color palettes.
#define SECONDS_PER_PALETTE  5
// Also: toward the bottom of the file is an array 
// called "ActivePaletteList" which controls which color
// palettes are used; you can add or remove color palettes
// from there freely.

// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.
CRGB gBackgroundColor = CRGB::Black; 
// Example of dim incandescent fairy light background color
// CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);

// If AUTO_SELECT_BACKGROUND_COLOR is set to 1,
// then for any palette where the first two entries 
// are the same, a dimmed version of that color will
// automatically be used as the background color.
#define AUTO_SELECT_BACKGROUND_COLOR 0

// If COOL_LIKE_INCANDESCENT is set to 1, colors will 
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1


CRGBPalette16 gCurrentPalette;
CRGBPalette16 gTargetPalette;


class Timer {
public:
  Timer()
    : start(millis()) {}
  Timer(uint32_t duration)
    : start(millis()), duration(duration) {}

  bool isExpired() {
    return (millis() - start) >= duration;
  }

  void reset() {
    start = millis();
  }

private:
  uint32_t start = 0;
  uint32_t duration = 0;
};

Timer twinklefox(1000*60*2);
void setup() {
  // Holiday Coro tells me the chip driving the lights is a UCS1903 but the
  // sticker on the strand says 2811/1903. Hmmmm. The UCS1903 setting works.
  //
  // The strand also needs a 5V data signal. If you have the power plugged into
  // the wall _while_ you're programming, all works well. If it's only plugged
  // into USB, the data signal comes out at 3.3V and the strand goes haywire.
  // Took me a full day to figure this out. Plug the stupid board into wall
  // power.
  randomSeed(analogRead(0));
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(128);
  initializeDefaults();
  showDefault();
  chooseNextColorPalette(gCurrentPalette);
  chooseNextColorPalette(gTargetPalette);
}


void loop() {

 
  // Delay between crazy stuff happening.
  // message("abcdefghijklmnopqrstuvwxyz");
  if (twinklefox.isExpired())
  {
    do_twinklefox(10000);
    twinklefox.reset();    
    fadeToDefault(10, 1000);

  }

  // sparkle(random(5,10)*1000);
  FastLED.delay(random(3, 7) * 1000);
  int behavior = random(0, 8);
  switch (behavior) {
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
      message("hello");
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

void sparkle(uint32_t duration) {
  // int start = millis();
  Timer overall(duration);
  Timer cycle(110);

  while (!overall.isExpired()) {
    for (int i = 0; i < NUM_LEDS; i++) {
      int on = random(1, 5);
      // if (on < 4) {
      //   leds[i] = defaultColors[i];
      // } else if (on < 6) {
      //   leds[i] = CRGB(defaultColors[i]).nscale8_video(32);
      // } else if (on < 8) {
      //   leds[i] = CRGB(defaultColors[i]).nscale8_video(16);
      // } else {
      //   leds[i] = CRGB::Black;
      // }
      leds[i] = blend(defaultColors[i], CRGB::Black, 255 / on);
      // leds[i] = on < 8 ? CRGB(defaultColors[i] : CRGB::Black;
    }
    cycle.reset();
    while (!cycle.isExpired()) {
      FastLED.show();
      delay(1);
    }
  }

  // sparkleOut();
  // FastLED.delay(1000);
  fadeToDefault(10, 1000);
}

void sparkleOut() {
  // Create a list of light indices.
  int indices[NUM_LEDS];
  for (int i = 0; i < NUM_LEDS; i++) {
    indices[i] = i;
  }

  // Shuffle the indices into a random order.
  int last = 0;
  int temp = indices[last];
  for (int i = 0; i < NUM_LEDS; i++) {
    int index = random(NUM_LEDS);
    indices[last] = indices[index];
    last = index;
  }

  indices[last] = temp;

  // Turn the lights off in the random order.
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[indices[i]] = CRGB::Black;
    FastLED.show();
    FastLED.delay(70);
  }

  FastLED.delay(1000);
  fadeToDefault(10, 1000);
}

void redpulse() {
  for (int i = 0; i < 3; i++) {
    fadeToColor(CRGB::Red, 20, 1000);
    fadeToColor(CRGB(64, 0, 0), 10, 500);
  }

  fadeToColor(CRGB::Red, 20, 1000);
  FastLED.delay(3000);
  fadeToDefault(20, 1000);
}

void fadeToDefault(int fadeSteps, int ms) {
  CRGB original[NUM_LEDS];
  for (int i = 0; i < NUM_LEDS; i++) {
    original[i] = CRGB(leds[i]);
  }

  for (int step = 1; step <= fadeSteps; step++) {
    // blend 'amount' is a fraction from 0 - 255
    float amount = ((float)step / fadeSteps) * 255;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = blend(original[i], defaultColors[i], amount);
    }

    FastLED.show();
    FastLED.delay(ms / fadeSteps);
  }
}

void fadeToColor(CRGB color, int fadeSteps, int ms) {
  CRGB original[NUM_LEDS];
  for (int i = 0; i < NUM_LEDS; i++) {
    original[i] = CRGB(leds[i]);
  }

  for (int step = 1; step <= fadeSteps; step++) {
    // blend 'amount' is a fraction from 0 - 255
    float amount = ((float)step / fadeSteps) * 255;
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = blend(original[i], color, amount);
    }

    FastLED.show();
    FastLED.delay(ms / fadeSteps);
  }
}

void message(char *text) {
  // A couple of "pops" of color before showing the message.
  fadeToColor(CRGB::Black, 10, 250);
  fadeToColor(CRGB::White, 10, 250);
  fadeToColor(CRGB::Black, 5, 100);
  fadeToColor(CRGB::White, 10, 500);
  fadeToColor(CRGB::Black, 10, 500);
  size_t length = strlen(text);
  for (int i = 0; i < length; i++) {
    int index = lookupChar(text[i]);
    if (index >= 0) {
      leds[index] = CRGB(defaultColors[index]);
      FastLED.show();
      FastLED.delay(1250);
      FastLED.clear();
      FastLED.show();
      FastLED.delay(400);
    } else {
      // Space, punctuation, etc.
      FastLED.delay(1050);
    }
  }

  fadeToDefault(20, 1000);

  sparkle(5000);
  fadeToDefault(20, 1000);
}

void showDefault() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(defaultColors[i]);
  }

  FastLED.show();
}

int lookupChar(char c) {
  switch (c) {
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

void initializeDefaults() {
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
  defaultColors[50] = CRGB::Red;
  defaultColors[51] = CRGB::Blue;
  defaultColors[52] = CRGB::Yellow;
  defaultColors[53] = CRGB::Red;
  defaultColors[54] = CRGB::Green;
  defaultColors[55] = CRGB::Yellow;
  defaultColors[56] = CRGB::Blue;
  defaultColors[57] = CRGB::Orange;
  defaultColors[58] = CRGB::Purple;
  defaultColors[59] = CRGB::Green;
  defaultColors[60] = CRGB::Red;
  defaultColors[61] = CRGB::Yellow;
  defaultColors[62] = CRGB::Blue;
  defaultColors[63] = CRGB::Red;
  defaultColors[64] = CRGB::Green;
  defaultColors[65] = CRGB::Purple;
  defaultColors[66] = CRGB::Yellow;
  defaultColors[67] = CRGB::Red;
  defaultColors[68] = CRGB::Green;
  defaultColors[69] = CRGB::Orange;
  defaultColors[70] = CRGB::Yellow;
  defaultColors[71] = CRGB::Green;
  defaultColors[72] = CRGB::Blue;
  defaultColors[73] = CRGB::Purple;
  defaultColors[74] = CRGB::Red;
  defaultColors[75] = CRGB::Green;
  defaultColors[76] = CRGB::Yellow;
  defaultColors[77] = CRGB::Blue;
  defaultColors[78] = CRGB::White;
  defaultColors[79] = CRGB::Green;
  defaultColors[80] = CRGB::Yellow;
  defaultColors[81] = CRGB::Blue;
  defaultColors[82] = CRGB::Purple;
  defaultColors[83] = CRGB::Red;
  defaultColors[84] = CRGB::Yellow;
  defaultColors[85] = CRGB::Yellow;
  defaultColors[86] = CRGB::Green;
  defaultColors[87] = CRGB::Blue;
  defaultColors[88] = CRGB::Purple;
  defaultColors[89] = CRGB::Red;
  defaultColors[90] = CRGB::Green;
  defaultColors[91] = CRGB::Yellow;
  defaultColors[92] = CRGB::Blue;
  defaultColors[93] = CRGB::White;
  defaultColors[94] = CRGB::Green;
  defaultColors[95] = CRGB::Yellow;
  defaultColors[96] = CRGB::Blue;
  defaultColors[97] = CRGB::Purple;
  defaultColors[98] = CRGB::Red;
  defaultColors[99] = CRGB::Yellow;
}

const CRGB strange_colors[] = {
  CRGB::Bisque, CRGB::MediumBlue, CRGB::Indigo, CRGB::GreenYellow, CRGB::DodgerBlue, CRGB::Goldenrod, CRGB::Indigo, CRGB::GreenYellow,
  CRGB::GreenYellow, CRGB::Indigo, CRGB::MediumBlue, CRGB::GreenYellow, CRGB::Goldenrod, CRGB::Indigo, CRGB::Indigo, CRGB::GreenYellow, CRGB::Indigo,
  CRGB::GreenYellow, CRGB::Bisque, CRGB::Goldenrod, CRGB::MediumBlue, CRGB::Indigo, CRGB::MediumBlue, CRGB::Goldenrod, CRGB::Indigo, CRGB::Indigo
};

void do_twinklefox(uint32 ms) {
  Timer duration(ms);

  chooseNextColorPalette( gTargetPalette ); 

  while (!duration.isExpired()) {
    
    EVERY_N_MILLISECONDS( 10 ) {
      nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 12);
    }

    drawTwinkles(leds);
     delay(1); 
    FastLED.show();

  }


}


//  This function loops over each pixel, calculates the 
//  adjusted 'clock' that this pixel should use, and calls 
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color, 
//  whichever is brighter.
void drawTwinkles( CRGBSet& L)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;
  
  uint32_t clock32 = millis();

  // Set up the background color, "bg".
  // if AUTO_SELECT_BACKGROUND_COLOR == 1, and the first two colors of
  // the current palette are identical, then a deeply faded version of
  // that color is used for the background color
  CRGB bg;
  if( (AUTO_SELECT_BACKGROUND_COLOR == 1) &&
      (gCurrentPalette[0] == gCurrentPalette[1] )) {
    bg = gCurrentPalette[0];
    uint8_t bglight = bg.getAverageLight();
    if( bglight > 64) {
      bg.nscale8_video( 16); // very bright, so scale to 1/16th
    } else if( bglight > 16) {
      bg.nscale8_video( 64); // not that bright, so scale to 1/4th
    } else {
      bg.nscale8_video( 86); // dim, scale to 1/3rd.
    }
  } else {
    bg = gBackgroundColor; // just use the explicitly defined background color
  }

  uint8_t backgroundBrightness = bg.getAverageLight();
  
  for( CRGB& pixel: L) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color, 
      // use the new color.
      pixel = c;
    } else if( deltabright > 0 ) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      pixel = blend( bg, c, deltabright * 8);
    } else { 
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      pixel = bg;
    }
  }
}


//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as 
//  input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt)
{
  uint16_t ticks = ms >> (8-TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  uint8_t bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8( fastcycle8);
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
    if( COOL_LIKE_INCANDESCENT == 1 ) {
      coolLikeIncandescent( c, fastcycle8);
    }
  } else {
    c = CRGB::Black;
  }
  return c;
}


// This function is like 'triwave8', which produces a 
// symmetrical up-and-down triangle sawtooth waveform, except that this
// function produces a triangle wave with a faster attack and a slower decay:
//
//     / \ 
//    /     \ 
//   /         \ 
//  /             \ 
//

uint8_t attackDecayWave8( uint8_t i)
{
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

// This function takes a pixel, and if its in the 'fading down'
// part of the cycle, it adjusts the color a little bit like the 
// way that incandescent bulbs fade toward 'red' as they dim.
void coolLikeIncandescent( CRGB& c, uint8_t phase)
{
  if( phase < 128) return;

  uint8_t cooling = (phase - 128) >> 4;
  c.g = qsub8( c.g, cooling);
  c.b = qsub8( c.b, cooling * 2);
}

// A mostly red palette with green accents and white trim.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedGreenWhite_p FL_PROGMEM =
{  CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, 
   CRGB::Red, CRGB::Red, CRGB::Gray, CRGB::Gray, 
   CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green };

// A mostly (dark) green palette with red berries.
#define Holly_Green 0x00580c
#define Holly_Red   0xB00402
const TProgmemRGBPalette16 Holly_p FL_PROGMEM =
{  Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Green, 
   Holly_Green, Holly_Green, Holly_Green, Holly_Red 
};

// A red and white striped palette
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 RedWhite_p FL_PROGMEM =
{  CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray,
   CRGB::Red,  CRGB::Red,  CRGB::Red,  CRGB::Red, 
   CRGB::Gray, CRGB::Gray, CRGB::Gray, CRGB::Gray };

// A mostly blue palette with white accents.
// "CRGB::Gray" is used as white to keep the brightness more uniform.
const TProgmemRGBPalette16 BlueWhite_p FL_PROGMEM =
{  CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, 
   CRGB::Blue, CRGB::Gray, CRGB::Gray, CRGB::Gray };

// A pure "fairy light" palette with some brightness variations
#define HALFFAIRY ((CRGB::FairyLight & 0xFEFEFE) / 2)
#define QUARTERFAIRY ((CRGB::FairyLight & 0xFCFCFC) / 4)
const TProgmemRGBPalette16 FairyLight_p FL_PROGMEM =
{  CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, 
   HALFFAIRY,        HALFFAIRY,        CRGB::FairyLight, CRGB::FairyLight, 
   QUARTERFAIRY,     QUARTERFAIRY,     CRGB::FairyLight, CRGB::FairyLight, 
   CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight, CRGB::FairyLight };

// A palette of soft snowflakes with the occasional bright one
const TProgmemRGBPalette16 Snow_p FL_PROGMEM =
{  0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0x304048,
   0x304048, 0x304048, 0x304048, 0xE0F0FF };

// A palette reminiscent of large 'old-school' C9-size tree lights
// in the five classic colors: red, orange, green, blue, and white.
#define C9_Red    0xB80400
#define C9_Orange 0x902C02
#define C9_Green  0x046002
#define C9_Blue   0x070758
#define C9_White  0x606820
const TProgmemRGBPalette16 RetroC9_p FL_PROGMEM =
{  C9_Red,    C9_Orange, C9_Red,    C9_Orange,
   C9_Orange, C9_Red,    C9_Orange, C9_Red,
   C9_Green,  C9_Green,  C9_Green,  C9_Green,
   C9_Blue,   C9_Blue,   C9_Blue,
   C9_White
};

// A cold, icy pale blue palette
#define Ice_Blue1 0x0C1040
#define Ice_Blue2 0x182080
#define Ice_Blue3 0x5080C0
const TProgmemRGBPalette16 Ice_p FL_PROGMEM =
{
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue1, Ice_Blue1, Ice_Blue1, Ice_Blue1,
  Ice_Blue2, Ice_Blue2, Ice_Blue2, Ice_Blue3
};


// Add or remove palette names from this list to control which color
// palettes are used, and in what order.
const TProgmemRGBPalette16* ActivePaletteList[] = {
  &RetroC9_p,
  &BlueWhite_p,
  &RainbowColors_p,
  &FairyLight_p,
  &RedGreenWhite_p,
  &PartyColors_p,
  &RedWhite_p,
  &Snow_p,
  &Holly_p,
  &Ice_p  
};


// Advance to the next color palette in the list (above).
void chooseNextColorPalette( CRGBPalette16& pal)
{
  const uint8_t numberOfPalettes = sizeof(ActivePaletteList) / sizeof(ActivePaletteList[0]);
  static uint8_t whichPalette = -1; 
  whichPalette = addmod8( whichPalette, 1, numberOfPalettes);

  pal = *(ActivePaletteList[whichPalette]);
}
