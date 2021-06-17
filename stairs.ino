#include <FastLED.h>

#define NUM_LEDS 100
#define LED_PIN 4
#define PIR_PIN 2 // AM312 PIR (motion sensor) pin

CRGB leds[NUM_LEDS];

int pirStat = 0; // PIR status
bool animating = false;

void setup() {
  pinMode(PIR_PIN, INPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setDither(0);
}

void loop() {

  if (animating == false) {

    pirStat = digitalRead(PIR_PIN);

    if (pirStat == HIGH) { // if motion detected
      animate();
    }

    delay(250);

  }

}

void animate() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(255, 255, 255);
    FastLED.show();
    leds[i].setRGB(0,0,0);
    delay(20);
  }

  FastLED.show();
  animating = false;
}
