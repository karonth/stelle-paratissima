/*
Lights display for paratissima

Based on inoise8_pal_demo code from Andrew Tuline
*/

#include "FastLED.h"

/*
There are 21 stars with 8 leds each totalling 168
*/
#define STARS 21
#define STAR_LEDS 8
#define NUM_LEDS STARS*STAR_LEDS
//#define NUM_LEDS 168

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN

//#APA102
//#define DATA_PIN 6
//#define CLOCK_PIN 5

//for ws2812b
#define DATA_PIN 7
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B   // Only use the LED_PIN for WS2812's
#define COLOR_ORDER GRB

// Define the array of leds
CRGB leds[NUM_LEDS];


static uint16_t dist;         // A random number for our noise generator.
uint16_t scale = 30;          // Wouldn't recommend changing this on the fly, or the animation will be really blocky.
uint8_t maxChanges = 48;      // Value for blending between palettes.

/*
#f8ca00
#fd0
gold
hsl(48,100,48)
rgb(248,202,0)
*/
CRGB rgb(248,202,0);          //gold rgb
static uint8_t baseHue = 48;  //gold hue


CRGBPalette16 currentPalette(CRGB::Black);
//CRGBPalette16 targetPalette(OceanColors_p);

CRGBPalette16 targetPalette(rgb);

void setup() { 
  Serial.begin(115200);

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  LEDS.setBrightness(BRIGHTNESS);
  dist = random16(12345);          // A semi-random number for our noise generator

  //FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);  
}

void loop() { 

  //testYellow();
  

  EVERY_N_MILLISECONDS(10) {
    nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);  // Blend towards the target palette
    fillnoise8();                                                           // Update the LED array with noise at the new location
  }
  
  EVERY_N_SECONDS(5) {             
  // Change the target palette to a random saturation every 5 seconds.
    targetPalette = CRGBPalette16(
      CHSV(baseHue, 255, random8(128,255)), 
      CHSV(baseHue, 255, random8(128,255)), 
      CHSV(baseHue, 255, random8(128,255)), 
      CHSV(baseHue, 255, random8(128,255)));
    //targetPalette = CRGBPalette16(RainbowColors_p);
  }
  
  LEDS.show();                    // Display the LED's at every loop cycle.
  
}


void fillnoise8() {
  for(int i = 0; i < NUM_LEDS; i++) {                                      // Just ONE loop to fill up the LED array as all of the pixels change.
    uint8_t index = inoise8(i*scale, dist+i*scale) % 255;                  // Get a value from the noise function. I'm using both x and y axis.
    leds[i] = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }
  dist += beatsin8(10,1, 4);                                               // Moving along the distance (that random number we started out with). Vary it a bit with a sine wave.
                                                                           // In some sketches, I've used millis() instead of an incremented counter. Works a treat.
} // fillnoise8()


void SetupPaletteFromHue(int hue)
{
  targetPalette = CRGBPalette16( 
                      CHSV( hue, 255, 32), 
                      CHSV( hue, 255, 255), 
                      CHSV( hue, 128, 255), 
                      CHSV( hue, 255, 255)); 
}



void ledswitch(CRGB color){
  // Turn the LED on, then pause
  for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = color;
  }
  FastLED.show();
}

void testYellow(){
  int on=1000;
  int off=500;
  /*
  #f8ca00
  #fd0
  gold
  hsl(48,100,48)
  rgb(248,202,0)
  */
  Serial.println("Gold");

  // Turn the LED on, then pause
  CRGB rgb(248,202,0);

  ledswitch(rgb);
  delay(on);
  ledswitch(CRGB::Black);
  delay(off);
}

void testrgb(){
 
  int on=5000;
  int off=1000;

  Serial.println("Red");
  ledswitch(CRGB::Red);
  delay(on);
  ledswitch(CRGB::Black);
  delay(off);

  Serial.println("Green");
  ledswitch(CRGB::Green);
  delay(on);
  ledswitch(CRGB::Black);
  delay(off);

  Serial.println("Blue");
  ledswitch(CRGB::Blue);
  delay(on);
  ledswitch(CRGB::Black);
  delay(off);

  Serial.println("White");
  ledswitch(CRGB::White);
  delay(on);
  ledswitch(CRGB::Black);
  delay(off);

}
