#include "gp_neopixel.h"

#define NEOPIXEL_PIN    9

Adafruit_NeoPixel strip(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

uint32_t BLACK  = strip.Color(0, 0, 0);
uint32_t RED    = strip.Color(255, 0, 0);
uint32_t GREEN  = strip.Color(0, 255, 0);
uint32_t BLUE   = strip.Color(0, 0, 255);
uint32_t CYAN   = strip.Color(0, 180, 255);
uint32_t YELLOW = strip.Color(255, 255, 0);
uint32_t PURPLE = strip.Color(180, 0, 255);
uint32_t GRAY   = strip.Color(80, 80, 80);
uint32_t WHITE  = strip.Color(255, 255, 255);

void gp_neopixel_init() {
  strip.begin();
  strip.setBrightness(1);
}

void gp_neopixel_set(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
  strip.setPixelColor(index,  strip.Color(r, g, b));
}

void gp_neopixel_show() {
  strip.show();
}


void gp_neopixel_set_half(uint32_t color) {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    strip.setPixelColor(i, i % 2 ? BLACK : color);
  }
  strip.show();
}
