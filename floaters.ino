#include <FastLED.h>
#define NUM_LEDS 144
#define LED_PIN 2
#define FRAMES_PER_SECOND  60
#define NUM_OF_FLOATERS 7
// #define BRIGHTNESS 50

CRGB leds[NUM_LEDS];

class Boris {
  public: Boris() {
    s = random(20, 100); // size -- or "radius"
    v = random(2, 12) / 20.0; // 2.0 has decimal to cast as float
    r = random(0, 255);
    g = 255;
    b = random(150, 200);
    x = -100;
  }
  public: float s; // size
  public: float v; // velocity
  public: float r; // red
  public: float g; // green
  public: float b; // blue
  public: float x; // position on x axis
};


Boris hi[NUM_OF_FLOATERS];

void setup() {

  // Serial.begin(9600);
  // Serial.println("Starting app!");

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  // FastLED.clear();
  // FastLED.show();
  // FastLED.setBrightness(BRIGHTNESS);
  FastLED.setDither(0);
}

void loop() {
  FastLED.clear();
  lol();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void lol() {
  fill();
  for (int z = 0; z < NUM_OF_FLOATERS; z++) {
    hi[z].x = hi[z].x + hi[z].v;
  }
}

void fill() {
  for (int z = 0; z < NUM_OF_FLOATERS; z++) {
    if (hi[z].x > NUM_LEDS + hi[z].s || hi[z].x < -hi[z].s) {
      createNew(z);
    }
    for (int i = floor(hi[z].x - hi[z].s); i < ceil(hi[z].x + hi[z].s); i++) {
      float brightnessScaler = sq(1 - abs(hi[z].x - i) / hi[z].s);
      float hueScaler = 1 - sq(abs(hi[z].x - i) / (hi[z].s));
      // Serial.println(scaler);
      if (i >= 0 && i < NUM_LEDS - 1) {
        leds[i] += CHSV(hi[z].r, max(hi[z].g * hueScaler, 1), max(min(hi[z].b * brightnessScaler, 200), 1));
      }
    }
  }

  setMinMax();
}

void setMinMax() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] += CHSV(1, 0, 1);
    leds[i].r = min(leds[i].r, 50);
    leds[i].g = min(leds[i].g, 50);
    leds[i].b = min(leds[i].b, 50);
  }
}

void createNew(int index) {
  bool direction = random(0, 2);
  hi[index].s = random(20, 100);
  hi[index].r = random(0, 255);
  if (direction) {
    hi[index].x = -hi[index].s;
    hi[index].v = random(2, 12) / 20.0;
  } else {
    hi[index].x = NUM_LEDS + hi[index].s;
    hi[index].v = -random(2, 12) / 20.0;
  }
}
