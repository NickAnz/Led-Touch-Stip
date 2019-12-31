#include <Arduino.h>
#include <FastLED.h>
#include <array> //used for sizeof()

#define NUM_LEDS 30
#define DATA_PIN 0
#define colourPin 12
#define confirmPin 4
#define contrastPin 5

using namespace std;

//configure the amount of LEDS in the strip and initialise them
//configure array of colour values to allow for selecting of colours
CRGB leds[NUM_LEDS];
const CRGB colorTypes[] = {CRGB::Red, CRGB::Orange, CRGB::Yellow, CRGB::Green, CRGB::Aqua, CRGB::Blue, CRGB::Purple, CRGB::Pink};
int colourState = 0;
int postionState = 30;

void setup()
{
  //set pinmode for all three input mics
  pinMode(colourPin, INPUT);
  pinMode(confirmPin, INPUT);
  pinMode(contrastPin, INPUT);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);
}

void pinColour()
{
  //sizeof refers to the size of the pointer not the array in order to do that we must find how many bits are in each value
  if (colourState != (sizeof(colorTypes) / sizeof(colorTypes[0]))-1)
  {
    colourState++;
    Serial.println(colourState);
    leds[postionState] = colorTypes[colourState];
    FastLED.show();
    delay(30);
  }
  else
  {
    colourState = -1;
  }
}

void ledPostion()
{
  if (postionState != 0)
  {
    //this for loop presents a trail effect on the light strip
    for (int i = 0; i < postionState; i++)
    {
      leds[i] = colorTypes[colourState];
      FastLED.show();
      leds[i] = CRGB::Black; //turn off the led when the next pass comes
    }
    //sets the last state led to be permanently on to the selected colourState
    leds[postionState] = colorTypes[colourState];
    FastLED.show();
    postionState--;
  }
  else
  {
    postionState = 30;
  }
}

void ravePin()
{
  for (int colorStep = 0; colorStep < 256; colorStep++)
  {

    int r = colorStep;       // Redness starts at zero and goes up to full
    int b = 255 - colorStep; // Blue starts at full and goes down to zero
    int g = 0;               // No green needed to go from blue to red

    // Now loop though each of the LEDs and set each one to the current color

    for (int x = 0; x < NUM_LEDS; x++)
    {
      leds[x] = CRGB(r, g, b);
    }
    FastLED.show();
    postionState= 30;
    
  }
}

void loop()
{

  if (digitalRead(colourPin) == 0)
  {
    pinColour();
    //Serial.println("ColourPin Active");
  }

  if (digitalRead(confirmPin) == 0)
  {
    ledPostion();
    //Serial.println("ConfirmPin Active");
  }
  if (digitalRead(contrastPin) == 0)
  {
    ravePin();
    // Serial.println("ContrastPin Active");
  }
}
