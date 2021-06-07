#include <FastLED.h>
#define NUM_LEDS 144
#define LED_PIN 2

#define MAX_BRIGHTNESS 50 // 255 max
#define FRAMES_PER_SECOND 30

#define NUM_OF_FLOATERS 3

CRGB leds[NUM_LEDS];

class Floater {
  public: 
    float r; // radius
    float x; // position on x axis
    float v; // velocity
    float hueDrift; // speed at which hue changes

    // COLOR:
    float hue; // Hue
    float saturation; // Saturation
    float brightness; // Brightness

    Floater() {
      x = -200; // seems to be needed
    }
};

Floater floaters[NUM_OF_FLOATERS];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setDither(0);

  for(int i; i < NUM_OF_FLOATERS; i++) {
    createNewFloater(i);
  }
}

void loop() {
  FastLED.clear();
 
  fill();
  setMinMax();
  updatePositions();
 
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void updatePositions() {
  for (int i = 0; i < NUM_OF_FLOATERS; i++) {
    floaters[i].x = floaters[i].x + floaters[i].v;
    floaters[i].hue = floaters[i].hue + floaters[i].hueDrift;
  }
}

void fill() {
  for (int z = 0; z < NUM_OF_FLOATERS; z++) {

    if (floaters[z].x > NUM_LEDS + floaters[z].r || floaters[z].x < -floaters[z].r) {
      createNewFloater(z);
    }

    // For all LEDs within the radius of x position, add HSL based on dropoff gradient
    int lowest = floor(floaters[z].x - floaters[z].r);
    int highest = ceil(floaters[z].x + floaters[z].r);

    for (int i = lowest; i < highest; i++) {

      float brightnessScaler = sq(1 - abs(floaters[z].x - i) / floaters[z].r);
      float hueScaler = 1 - sq(abs(floaters[z].x - i) / (floaters[z].r));

      if (i >= 0 && i < NUM_LEDS - 1) {
        leds[i] += CHSV(floaters[z].hue, 
                        max(floaters[z].saturation * hueScaler, 1), 
                        max(min(floaters[z].brightness * brightnessScaler, 200), 1));
      }
    }

  }
}

// set minimum and maximum brightness
void setMinMax() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] += CHSV(0, 0, 1); // minimum brightness = 1
    leds[i].r = min(leds[i].r, MAX_BRIGHTNESS);
    leds[i].g = min(leds[i].g, MAX_BRIGHTNESS);
    leds[i].b = min(leds[i].b, MAX_BRIGHTNESS);
  }
}

void createNewFloater(int i) {
  floaters[i].r = random(20, 100);
  floaters[i].v = random(2, 12) / 10.0;
  floaters[i].hue = random(0, 255);
  floaters[i].saturation = random(150, 255);
  floaters[i].brightness = random(100, 200);
  floaters[i].hueDrift = random(0, 10) / 20.0;

  bool direction = random(0, 2);

  if (direction) {
    floaters[i].x = -floaters[i].r;
  } else {
    floaters[i].x = NUM_LEDS + floaters[i].r;
    floaters[i].v = -floaters[i].v;
  }
}
